/*
 * Copyright Abe Takafumi All Rights Reserved, 2004-2016
 * Author Abe Takafumi
 *
 */

////////////////////////////////////////////////////////////////////////
// linux / gcc
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
	int cpu_num = (int)pparam;

	sharaku_cpu_handler();
	pthread_exit(NULL);
}

int sharaku_entry(void)
{
	int			i;
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
		pthread_create(&thread, &thread_attr, _cpu_handler, i);
	}
	pthread_create(&thread, &thread_attr, _timer_handler, i);

	sharaku_cpu_handler();

	// sharakuの破棄処理
	_sharaku_finl();

	return sharaku_get_exit_message();
}

