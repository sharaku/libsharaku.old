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
#include <sharaku/task.h>
#include <sharaku/module.h>
#include <sharaku/activity.h>
#include <sharaku/prof.h>
#include <sharaku/log.h>
#include "task-in.h"

#if 0
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
#include <sys/timerfd.h>
////////////////////////////////////////////////////////////////////////
#elif defined(__itron__)
////////////////////////////////////////////////////////////////////////
// toppers / gcc
#include <itron.h>
#include <kernel.h>
#include <svc_call.h>
#endif

#endif

/******************************************************************************
extern section
******************************************************************************/

/******************************************************************************
global value section
******************************************************************************/
static volatile int static_exit_flag = 0;
static volatile int static_exit_code = 0;
static sharaku_usec_t static_start_ms = 0;

/******************************************************************************
function section
******************************************************************************/
void sharaku_exit_message(int result)
{
	static_exit_code = result;
	static_exit_flag = 1;
}

void sharaku_initialize(void)
{
	static_start_ms = sharaku_get_usec() / 1000;

	// sharakuの初期化
	sharaku_dblog_initialize();
	sharaku_prof_initialize();
	sharaku_initialize_scheduler();
	sharaku_module_initialize();
	sharaku_activity_initialize();
}
void sharaku_finalize(void)
{
	// sharakuの破棄処理
	sharaku_activity_finalize();
	sharaku_module_finalize();
	sharaku_finalize_scheduler();
	sharaku_prof_finalize();
	sharaku_dblog_finalize();
}
void sharaku_timer_handler(void)
{
	sharaku_usec_t  msec;

	msec = sharaku_get_usec() / 1000;
	sharaku_entry_job_timer((uint32_t)msec - (uint32_t)static_start_ms);
	static_start_ms = msec;
}

int sharaku_get_exit_message(void)
{
	return static_exit_code;
}

int sharaku_is_exit(void)
{
	return static_exit_flag;
}

// include先は、OS, プラットフォーム, アーキテクチャによって
// 適切に切り替わる。
#include <sched-in.c>

