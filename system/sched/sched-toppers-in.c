/*****************************************************************************/
/*!
@author	abe takafumi
@par	Copyright
2004-2014 abe takafumi All Rights Reserved.
@addtogroup core
@file	scheduler.c
@brief	プラットフォーム固有スケジューラ機能
	対応プラットフォーム：Windows (C++), Linux (C/C++) ...
******************************************************************************/

/******************************************************************************
include section
******************************************************************************/
#include <sharaku/job.h>
#include <sharaku/module.h>
#include <sharaku/activity.h>
#include <sharaku/prof.h>
#include <sharaku/log.h>
#include "job-in.h"

#if defined(WIN32)
////////////////////////////////////////////////////////////////////////
// windows
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN	// windows.hから使用されていない部分を除外
// Windows ヘッダー ファイル:
#include <windows.h>

// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// 高精度タイマ ヘッダ/ライブラリファイル
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib")	//  WinMMAPIライブラリ

////////////////////////////////////////////////////////////////////////
#elif defined(__linux__)
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
////////////////////////////////////////////////////////////////////////
#elif defined(__itron__)
////////////////////////////////////////////////////////////////////////
// toppers / gcc
#include <itron.h>
#include <kernel.h>
#include <svc_call.h>
#endif

/******************************************************************************
extern section
******************************************************************************/

/******************************************************************************
global value section
******************************************************************************/
static volatile int static_exit_flag = 0;
static volatile int static_exit_code = 0;
static sharaku_usec_t static_static_ms = 0;
static volatile uint32_t static_timer_interrupt = 0;

/******************************************************************************
function section
******************************************************************************/
void sharaku_exit_message(int result)
{
	static_exit_code = result;
	static_exit_flag = 1;
}

static void sharaku_initialize(void)
{
	// sharakuの初期化
	sharaku_dblog_initialize();
	sharaku_prof_initialize();
	sharaku_initialize_scheduler();
	sharaku_module_initialize();
	sharaku_activity_initialize();
}
static void sharaku_finalize(void)
{
	// sharakuの破棄処理
	sharaku_activity_finalize();
	sharaku_module_finalize();
	sharaku_finalize_scheduler();
	sharaku_prof_finalize();
	sharaku_dblog_finalize();
}
static void sharaku_timer_handler(void)
{
	sharaku_usec_t  msec;

	msec = sharaku_get_usec() / 1000;
	sharaku_entry_job_timer((uint32_t)msec - (uint32_t)static_static_ms);
	static_static_ms = msec;
}

#if defined(WIN32)
////////////////////////////////////////////////////////////////////////
// windows
////////////////////////////////////////////////////////////////////////
HANDLE g_timeout_event;
static void CALLBACK timer_callback(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	sharaku_usec_t  msec;

	msec = sharaku_get_usec() / 1000;
	sharaku_entry_job_timer((uint32_t)msec - (uint32_t)static_static_ms);
	static_static_ms = msec;
}

static DWORD WINAPI _thread_handler(LPVOID pparam)
{
	struct sharaku_job_context*	context
				 = (struct sharaku_job_context*)pparam;

	// createパラメータを元にスケジューラをスケジューラを起動する
	// 停止指示もしくはアプリ終了指示があった場合は、threadを終了する。
	while (!context->is_stop && static_exit_flag != 1) {
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
		context->thread[i] = _beginthread(_thread_handler, 0 , context);
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

static int _sharaku_entry(void)
{
	MMRESULT	res;

	static_static_ms = sharaku_get_usec() / 1000;

	// 高精度タイマーの生成
	res = timeSetEvent(1, 0, timer_callback, (DWORD)0, TIME_PERIODIC);

	// sharakuの初期化
	sharaku_initialize();

	while (static_exit_flag != 1) {
		sharaku_entry_job_cycle();
	}

	// 高精度タイマーの破棄
	timeKillEvent(res);

	// sharakuの破棄処理
	sharaku_finalize();

	return static_exit_code;
}
#elif defined(__linux__)
////////////////////////////////////////////////////////////////////////
// linux/gcc
////////////////////////////////////////////////////////////////////////
static void _signal_alrm_handler(int signum)
{
	// タイマがハードウェア割り込みの場合、本延長でタイマを
	// スケジュールすると、停止するため、メインスケジューラ側で
	// スケジュールする
	static_timer_interrupt = 1;
}

static void* _thread_handler(void* pparam)
{
	struct sharaku_job_context*	context
				 = (struct sharaku_job_context*)pparam;

	// createパラメータを元にスケジューラをスケジューラを起動する
	// 停止指示もしくはアプリ終了指示があった場合は、threadを終了する。
	while (!context->is_stop && static_exit_flag != 1) {
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


static int _sharaku_entry(void)
{
	struct sigaction	action = {
		.sa_handler	= _signal_alrm_handler,
		.sa_flags	= SA_RESTART
	};
	struct itimerval	timer = {
		.it_value.tv_sec	= 0,
		.it_value.tv_usec	= 1000,
		.it_interval.tv_sec	= 0,
		.it_interval.tv_usec	= 1000,
	};
	static_static_ms	= sharaku_get_usec() / 1000;

	sigemptyset(&action.sa_mask);
	if(sigaction(SIGALRM, &action, NULL) < 0) {
		perror("sigaction error");
		exit(1);
	}

	// sharakuの初期化
	sharaku_initialize();

	/* set intarval timer (FRAMEWORK_TIMER_RATE) */
	if(setitimer(ITIMER_REAL, &timer, NULL) < 0) {
		perror("setitimer error");
		exit(1);
	}

	while (static_exit_flag != 1) {
		if (static_timer_interrupt) {
			sharaku_timer_handler();
			static_timer_interrupt = 0;
		}
		sharaku_entry_job_cycle(sharaku_get_default_context());
	}

	// sharakuの破棄処理
	sharaku_finalize();

	return static_exit_code;
}
#elif defined(__itron__)
////////////////////////////////////////////////////////////////////////
// toppers/gcc
////////////////////////////////////////////////////////////////////////
static void _signal_alrm_handler(VP_INT signum)
{
	// タイマがハードウェア割り込みの場合、本延長でタイマを
	// スケジュールすると、停止するため、メインスケジューラ側で
	// スケジュールする
	static_timer_interrupt = 1;
}


static void _thread_handler(void* pparam)
{
}

int create_scheduler(struct sharaku_create_scheduler* param)
{
	// itronはデフォルトスケジューラ以外禁止
	// 現状必要はない。
	// ToDo: 必要なになったら対応
	return -1;
}

static int _sharaku_entry(void)
{
	ER_ID	ercd;
	T_CALM	calm;
	ID	id_alm = 0;

	calm.almatr	= TA_HLNG;
	calm.exinf	= (intptr_t)NULL;
	calm.almhdr	= _signal_alrm_handler;
	ercd = acre_alm(&calm);
	if (ercd > 0)
		id_alm = ercd;

	// sharakuの初期化
	sharaku_initialize();

	// 1msごとに起動する
	sta_alm(id_alm, 1);

	while (static_exit_flag != 1) {
		if (static_timer_interrupt) {
			sharaku_timer_handler();
			static_timer_interrupt = 0;
		}
		sharaku_entry_job_cycle(sharaku_get_default_context());
	}

	// sharakuの破棄処理
	sharaku_finalize();

	del_alm(id_alm);

	return static_exit_code;
}
#endif

int sharaku_entry(void)
{
	return _sharaku_entry();
}
