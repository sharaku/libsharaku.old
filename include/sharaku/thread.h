/*****************************************************************************/
/*!
@author	abe takafumi
@par	Copyright
2014 abe takafumi All Rights Reserved.
@file	thread.h
@brief	threadを汎用的に使用できるようにする
	対応プラットフォーム：Windows (C++), Linux (C/C++) ...
******************************************************************************/
#ifndef _SHARAKU_THREAD_H
#define _SHARAKU_THREAD_H

#include <stdint.h>
#if defined(WIN32)
////////////////////////////////////////////////////////////////////////
// windows
////////////////////////////////////////////////////////////////////////
#include <windows.h>

// スレッド操作関数
typedef HANDLE		sharaku_thread_t;

// 同期関数: mutex
typedef CRITICAL_SECTION	sharaku_mutex_t;

#define sharaku_mutex_init(mx, ...)	InitializeCriticalSection(mx)
#define sharaku_mutex_destroy(mx, ...)	DeleteCriticalSection(mx)
#define sharaku_mutex_lock(mx,...)	EnterCriticalSection(mx)
#define sharaku_mutex_trylock(mx,...)	TryEnterCriticalSection(mx)
#define sharaku_mutex_unlock(mx,...)	LeaveCriticalSection(mx)

// 同期関数: 条件変数
typedef HANDLE			sharaku_cond_t;

#define sharaku_cond_init(cond, ...)
#define sharaku_cond_destroy(cond, ...)
#define sharaku_cond_signal(cond, ...)
#define sharaku_cond_broadcast(cond, ...)
#define sharaku_cond_wait(cond, ...)


#elif defined(__linux__)
////////////////////////////////////////////////////////////////////////
// linux/gcc
////////////////////////////////////////////////////////////////////////
#include <pthread.h>

// スレッド操作関数
typedef pthread_t		sharaku_thread_t;

// 同期関数: mutex
typedef pthread_mutex_t		sharaku_mutex_t;

#define sharaku_mutex_init(mx, ...)		pthread_mutex_init(mx, NULL)
#define sharaku_mutex_destroy(mx, ...)		pthread_mutex_destroy(mx)
#define sharaku_mutex_lock(mx,...)		pthread_mutex_lock(mx)
#define sharaku_mutex_trylock(mx,...)		pthread_mutex_trylock(mx)
#define sharaku_mutex_unlock(mx,...)		pthread_mutex_unlock(mx)

// 同期関数: 条件変数
typedef pthread_cond_t	sharaku_cond_t;

#define sharaku_cond_init(cond, ...)		pthread_cond_init(cond, NULL)
#define sharaku_cond_destroy(cond, ...)		pthread_cond_destroy(cond)
#define sharaku_cond_signal(cond, ...)		pthread_cond_signal(cond)
#define sharaku_cond_broadcast(cond, ...)	pthread_cond_broadcast(cond)
#define sharaku_cond_wait(cond, mx, ...)	pthread_cond_wait(cond, mx)

#elif defined(__itron__)
////////////////////////////////////////////////////////////////////////
// toppers/gcc
////////////////////////////////////////////////////////////////////////
#include <itron.h>
#include <kernel.h>
#include <svc_call.h>

// スレッド操作関数
typedef ER_ID			sharaku_thread_t;

// 同期関数: mutex
typedef ER_ID			sharaku_mutex_t;

#define sharaku_mutex_init(mx, ...)		{T_CMTX cmtx = {TA_TFIFO, 0}; *mx = acre_mtx(&cmtx);}
#define sharaku_mutex_destroy(mx, ...)		del_mtx(*mx)
#define sharaku_mutex_lock(mx,...)		loc_mtx(*mx)
#define sharaku_mutex_trylock(mx,...)		tloc_mtx(*mx)
#define sharaku_mutex_unlock(mx,...)		unl_mtx(*mx)

// 同期関数: 条件変数
typedef ER_ID			sharaku_cond_t;

#define sharaku_cond_init(cond, ...)		{T_CFLG cflg = {TA_TFIFO, 0}; *cond = acre_flg(&cflg);}
#define sharaku_cond_destroy(cond, ...)		del_flg(*cond)
#define sharaku_cond_signal(cond, ...)		set_flg(*cond, 0x01)
#define sharaku_cond_broadcast(cond, ...)	set_flg(*cond, 0x01)
#define sharaku_cond_wait(cond, mx, ...)	wai_flg(*cond, 0x01, TWF_ANDW, NULL)

#endif

#endif
