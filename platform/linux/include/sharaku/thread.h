﻿/* --
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
