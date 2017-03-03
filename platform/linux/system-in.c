/* --
 *
 * MIT License
 * 
 * Copyright (c) 2014-2016 Abe Takafumi
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

#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <poll.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <sharaku/system.h>

#define WORKER_THREADS 1
#define JIFFIES_HZ 1000000

static void* _timer_handler(void* pparam)
{
	int64_t	start_us, now_ms, now_us,diff_ms;

	// start_us が now_ms = 0の情報。
	// 差分は、(now_us - start_us) / 1000
	start_us = sharaku_get_usec();
	while (!_is_exit()) {
		usleep(JIFFIES_HZ / 1000);
		now_us = sharaku_get_usec();
		now_ms = sharaku_get_msec();
		diff_ms = (now_us - start_us) / 1000 - now_ms;
		sharaku_timer_handler(diff_ms);
	}

	pthread_exit(NULL);
}

static void* _cpu_handler(void* pparam)
{
//	int64_t cpu_num = (int)pparam;

	// ToDo
	// ここでCPUアフィニティを設定し、特定のCPUコアでのみ動作するように
	// 設定を行う。

	sharaku_cpu_handler();
	pthread_exit(NULL);
}

int sharaku_entry(void)
{
	int64_t			i;
	pthread_attr_t		thread_attr;
	pthread_t		thread;

	// sharakuの初期化
	_sharaku_init();

	// メインプロセスが使用可能なCPUに割り当てるスレッドを生成する。
	// メインスレッドはCPU 0固定で使用するため、それ以外のCPUを指定する。
	// スレッドに関しては、作りっぱなしにして、プロセス消滅時の開放
	// に任せる。
	// エラーしても、ワーカースレッドが0になるわけではないため、突き進む。
	pthread_attr_init(&thread_attr);
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	for (i = 1; i < WORKER_THREADS ; i++) {
		pthread_create(&thread, &thread_attr, _cpu_handler, (void *)i);
	}
	pthread_create(&thread, &thread_attr, _timer_handler, (void *)i);

	sharaku_cpu_handler();

	// sharakuの破棄処理
	_sharaku_finl();

	return sharaku_get_exit_message();
}

