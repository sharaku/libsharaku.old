/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */
#ifndef _SHARAKU_THREAD_H
#define _SHARAKU_THREAD_H

#include <stdint.h>
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

#endif
