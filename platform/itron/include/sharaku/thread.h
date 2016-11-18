/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */
#ifndef _SHARAKU_THREAD_H
#define _SHARAKU_THREAD_H

#include <stdint.h>
////////////////////////////////////////////////////////////////////////
// toppers/gcc
////////////////////////////////////////////////////////////////////////
#if 0
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
#else
typedef uint32_t		sharaku_thread_t;
typedef uint32_t		sharaku_mutex_t;
#define sharaku_mutex_init(mx, ...)
#define sharaku_mutex_destroy(mx, ...)
#define sharaku_mutex_lock(mx,...)
#define sharaku_mutex_trylock(mx,...)
#define sharaku_mutex_unlock(mx,...)
typedef uint32_t		sharaku_cond_t;
#define sharaku_cond_init(cond, ...)
#define sharaku_cond_destroy(cond, ...)
#define sharaku_cond_signal(cond, ...)
#define sharaku_cond_broadcast(cond, ...)
#define sharaku_cond_wait(cond, mx, ...)

#endif

#endif
