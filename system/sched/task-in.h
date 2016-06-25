/*****************************************************************************/
/*!
@author	abe takafumi
@par	Copyright
2004-2014 abe takafumi All Rights Reserved.
@addtogroup core
@file	job-in.h
@brief	job機能外部非公開ヘッダ
	対応プラットフォーム：Windows (C++), Linux (C/C++) ...
******************************************************************************/
#ifndef SHARAKU_JOB_IN_H_
#define SHARAKU_JOB_IN_H_

/******************************************************************************
include section
******************************************************************************/
#include <stdint.h>
#include <sharaku/list.h>
#include <sharaku/thread.h>
#include <sharaku/prof.h>

#define SHARAKU_CONTEXT_MAX	16

struct sharaku_job_context {
	uint8_t			is_initialize:1;
	uint8_t			is_stop:1;
	uint8_t			rsv:7;
	struct list_head	list;
	sharaku_mutex_t		mx;
	sharaku_cond_t		cond;
	uint32_t		base_time;
	uint32_t		threads;
	sharaku_thread_t	thread[0];
};

#if defined(__cplusplus)
extern "C" {
#endif

extern struct sharaku_job_context* sharaku_get_default_context(void);
extern struct sharaku_job_context* sharaku_get_multi_context(void);
extern struct sharaku_job_context* sharaku_get_context(int sid);
extern int sharaku_alloc_context(int threads);
extern void sharaku_free_context(struct sharaku_job_context* context);
extern void sharaku_initialize_scheduler(void);
extern void sharaku_finalize_scheduler(void);
extern void sharaku_entry_job_cycle(struct sharaku_job_context* context);
extern void sharaku_entry_job_timer(uint32_t millisecond);

#if defined(__cplusplus)
}
#endif

#endif // SHARAKU_JOB_IN_H_
