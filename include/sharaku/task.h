/*****************************************************************************/
/*!
@author	abe takafumi
@par	Copyright
2004-2014 abe takafumi All Rights Reserved.
@file	job.h
@brief	job機能
	対応プラットフォーム：Windows (C++), Linux (C/C++) ...
******************************************************************************/
#ifndef _SHARAKU_TASK_H_
#define _SHARAKU_TASK_H_

/******************************************************************************
include section
******************************************************************************/
#include <stdint.h>
#include <sharaku/list.h>
#include <sharaku/thread.h>
#include <sharaku/prof.h>

enum SHARAKU_JOB_STATUS {
	SHARAKU_JOB_STATUS_STOP,	//< 動作していない
	SHARAKU_JOB_STATUS_QUEUING,	//< 動作待ち
	SHARAKU_JOB_STATUS_ACTIVE,	//< 動作中
	SHARAKU_JOB_STATUS_SUSPEND,	//< TimeOutキューにつながる状態
};

struct sharaku_job;
typedef void(*sharaku_job_stagefunc_t)(struct sharaku_job*);


struct sharaku_job {
	struct list_head	list;		///< スケジューラのリンク
	sharaku_job_stagefunc_t	callback;	///< コールバック
	uint32_t		milli_sec;	///< タイムアウト値
	struct sharaku_job_context*
				context;	///< スケジューラコンテキスト
	uint8_t			status;		///< 現在の状態
	uint8_t			is_timeout:1;	///< タイムアウトしているか
	uint8_t			rsv_flg:7;	///< 
	uint32_t		magic;		// マジックコード
};

struct sharaku_create_scheduler {
	uint32_t		threads;	///< スレッド数
};

#define SHARAKU_TASK_MAGIC	0xF58AABE3

#if defined(__cplusplus)
extern "C" {
#endif

extern int create_scheduler(struct sharaku_create_scheduler* param);

extern void sharaku_init_job(struct sharaku_job *job);
extern int sharaku_set_jobparam(struct sharaku_job *job, int sid);
extern void sharaku_async_message(struct sharaku_job *job, sharaku_job_stagefunc_t callback);
extern void sharaku_sync_message(struct sharaku_job *job, sharaku_job_stagefunc_t callback);
extern void sharaku_timer_message(struct sharaku_job *job, uint32_t millisecond, sharaku_job_stagefunc_t callback);
extern void sharaku_cancel_message(struct sharaku_job *job);

#if defined(__cplusplus)
}
#endif


#endif /* _SHARAKU_TASK_H_ */
