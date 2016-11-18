/*
 * Copyright Abe Takafumi All Rights Reserved, 2004-2016
 * Author Abe Takafumi
 *
 */

////////////////////////////////////////////////////////////////////////
// linux / gcc
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <memory.h>
#include <sys/timerfd.h>

static void* _thread_handler(void* pparam)
{
	struct sharaku_job_context*	context
				 = (struct sharaku_job_context*)pparam;

	// createパラメータを元にスケジューラをスケジューラを起動する
	// 停止指示もしくはアプリ終了指示があった場合は、threadを終了する。
	while (!context->is_stop && !sharaku_is_exit()) {
		sharaku_entry_job_cycle(context);
	}

	pthread_exit(NULL);
}

int create_scheduler(struct sharaku_create_scheduler* param)
{
	int i;
	int ret;
	struct sharaku_job_context*	context;
	int sid;

	// 新規スケジューラコンテキストを確保 & 登録
	sid = sharaku_alloc_context(param->threads);
	context = sharaku_get_context(sid);
	if (!context) {
		goto err1;
	}

	// 必要数のスレッドを生成
	for (i = 0; i < param->threads; i++) {
		ret = pthread_create(&(context->thread[i]),
					NULL, _thread_handler, context);
		if (ret != 0) {
			goto err2;
		}
	}

	return sid;

 err2:
	// スレッドを破棄
	context->is_stop = 1;
	for (i = 0; i < param->threads; i++) {
		if (context->thread[i] != 0) {
			pthread_join(context->thread[i], NULL);
		}
	}

	// スケジューラコンテキストを破棄
	sharaku_free_context(context);

 err1:
	return -1;
}


int sharaku_entry(void)
{
	struct itimerspec	timer = {
		.it_interval.tv_sec	= 0,
		.it_interval.tv_nsec	= 1000000,
	};
	int		timerfd = 0;
	uint64_t	exp;
	ssize_t		s;

	timerfd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
	if(timerfd < 0) {
		perror("sigaction error");
		exit(1);
	}

	// sharakuの初期化
	sharaku_initialize();

	if(timerfd_settime(timerfd, TFD_TIMER_ABSTIME, &timer, NULL) < 0) {
		perror("setitimer error");
		exit(1);
	}

	while (!sharaku_is_exit()) {
		s = read(timerfd, &exp, sizeof(uint64_t));
		if (s != sizeof(uint64_t)) {
			if(timerfd_settime(timerfd, TFD_TIMER_ABSTIME, &timer, NULL) < 0) {
				perror("setitimer error");
				exit(1);
			}
			sharaku_timer_handler();
		}
		sharaku_entry_job_cycle(sharaku_get_default_context());
	}

	// sharakuの破棄処理
	sharaku_finalize();

	return sharaku_get_exit_message();
}

