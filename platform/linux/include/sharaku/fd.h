/* --
 *
 * MIT License
 * 
 * Copyright (c) 2017 Abe Takafumi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef _SHARAKU_FD_H_
#define _SHARAKU_FD_H_

#include <poll.h>
#include <errno.h>

// ファイルディスクリプタリストを監視し、イベントがあった場合は
// コールバックする。

struct fd_operations {
	int (*count_fd)(void);
	int (*add_fd)(struct pollfd *pfd, int cnt);
	int (*do_fd)(struct pollfd *pfd, int cnt);
};

// timeout 時間はシステムクロックの粒度に切り上げられ、 カーネルの
// スケジューリング遅延により少しだけ長くなる可能性がある点に注意すること。
// timeout に負の値を指定した場合、タイムアウト時間が無限大を意味する。
// timeout を 0 に指定した場合、I/O 可能なファイルディスクリプターがない場合
// であっても、 fd_poll() はすぐに返る。 
static inline int
fd_poll(struct fd_operations *ops, int cnt, int timeout)
{
	int i;
	int fdcnt = 0;
	int fdcnt2 = 0;
	struct pollfd	*pfd;
	int rc = 0;

	// リストに対してfd数を問い合わせる
	// 応答された数が監視対象のfd数。
	// スタックから取れない場合もあるので、mallocを使用する。
	for (i = 0, fdcnt = 0; i < cnt; i++) {
		fdcnt += ops->count_fd();
	}
	pfd = malloc(sizeof(struct pollfd) * fdcnt);
	for (i = 0, fdcnt2 = 0; i < cnt; i++) {
		fdcnt2 += ops->add_fd(&(pfd[fdcnt2]), fdcnt - fdcnt2);
	}

	// EINTRであればループを繰り返す。それ以外であればループ終了。
retry:
	rc = poll(pfd, fdcnt2, timeout);
	if (rc == -1 && errno == EINTR) {
		goto retry;
	}

	for (i = 0, fdcnt2 = 0; i < cnt; i++) {
		fdcnt2 += ops->do_fd(&(pfd[fdcnt2]), fdcnt - fdcnt2);
	}
	free(pfd);
	return 0;
}



#endif // _SHARAKU_FD_H_
