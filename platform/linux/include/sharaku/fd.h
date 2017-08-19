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
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ファイルディスクリプタリストを監視し、イベントがあった場合は
// コールバックする。

struct fd_operations {
	int (*count_fd)(void *);
	int (*add_fd)(void *, struct pollfd *, int);
	int (*do_fd)(void *, struct pollfd *, int);
	void *_this;
};

#define FD_OPERATIONS_INIT(C, A, D, THIS) {C, A, D, THIS}
static inline void
INIT_FD_OPERATIONS(struct fd_operations *ops,
		   int (*count_fd)(void *),
		   int (*add_fd)(void *, struct pollfd *, int),
		   int (*do_fd)(void *, struct pollfd *, int),
		   void *_this)
{
	ops->count_fd = count_fd;
	ops->add_fd = add_fd;
	ops->do_fd = do_fd;
	ops->_this = _this;
}

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
		fdcnt += ops->count_fd(ops->_this);
	}
	pfd = (struct pollfd*)malloc(sizeof(struct pollfd) * fdcnt);
	for (i = 0, fdcnt2 = 0; i < cnt; i++) {
		fdcnt2 += ops->add_fd(ops->_this, &(pfd[fdcnt2]), fdcnt - fdcnt2);
	}

	// EINTRであればループを繰り返す。それ以外であればループ終了。
retry:
	rc = poll(pfd, fdcnt2, timeout);
	if (rc == -1 && errno == EINTR) {
		goto retry;
	}

	for (i = 0, fdcnt2 = 0; i < cnt; i++) {
		fdcnt2 += ops->do_fd(ops->_this, &(pfd[fdcnt2]), fdcnt - fdcnt2);
	}
	free(pfd);
	return 0;
}

// タイマーFD制御
// 
// 使い方：
// 	#include <sharaku/fd.h>
// 	
// 	void
// 	timer_cb(void *this)
// 	{
// 	}
// 	
// 	struct timerfd_operations tfd_ope[] = {
// 		{timer_cb, NULL}
// 	};
// 	
// 	struct timerfd_info _timerinfo;
// 	struct fd_operations _fdope;
// 	
// 	int
// 	main(void)
// 	{
// 		timerfd_creat_fd(&_timerinfo, 1000, tfd_ope, 1);
// 		INIT_FD_OPERATIONS(&_fdope, timerfd_count_fd,
// 				timerfd_add_tfd, timerfd_do_fd, &_timerinfo);
//		fd_poll(&_fdope, 1, -1);
// 	}
// 	

// fd_poll用タイマーFDを監視、コールバック
// timerfd_operationsを初期化し、thisとして登録すること。
struct timerfd_operations
{
	void		(*t_cb)(void *);
	void		*_this;
};
// fd_poll用タイマーFDを監視、コールバック
// timerfd_operationsを初期化し、thisとして登録すること。
struct timerfd_info
{
	int				t_fd;
	uint32_t			t_ms;
	int				t_ops_num;
	struct timerfd_operations	*t_ops;
};

extern int timerfd_create_fd(struct timerfd_info *_this, uint32_t interval_ms,
			 struct timerfd_operations *ops, int opsnum);
extern int timerfd_destroy_fd(struct timerfd_info *_this);
extern int timerfd_count_fd(void *_this);
extern int timerfd_add_tfd(void *_this, struct pollfd *pfd, int cnt);
extern int timerfd_do_fd(void *_this, struct pollfd *pfd, int cnt);

// inotify
// ファイルを監視し、イベントが発生したらコールバックする。
struct inotify_operations
{
	void		(*i_cb)(void *);
	int		i_mask;
	void		*_this;
};
struct inotify_info
{
	int				i_fd;
	int				i_ops_num;
	struct inotify_operations	*i_ops;
};

// fd_poll用inotify監視、コールバック
extern int inotify_create_fd(struct inotify_info *_this,
			 struct inotify_operations *ops, int opsnum);
extern int inotify_destroy_fd(struct timerfd_info *_this);
extern int inotify_count_fd(void *_this);
extern int inotify_add_fd(void *_this, struct pollfd *pfd, int cnt);
extern int inotify_do_fd(void *_this, struct pollfd *pfd, int cnt);

// fd_poll用signalfd監視、コールバック
extern int signalfd_count_fd(void *_this);
extern int signalfd_add_fd(void *_this, struct pollfd *pfd, int cnt);
extern int signalfd_do_fd(void *_this, struct pollfd *pfd, int cnt);

#ifdef __cplusplus
}
#endif

#endif // _SHARAKU_FD_H_
