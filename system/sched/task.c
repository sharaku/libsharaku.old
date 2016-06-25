/*****************************************************************************/
/*!
@author	abe takafumi
@par	Copyright
2004-2014 abe takafumi All Rights Reserved.
@addtogroup core
@file	job.c
@brief	job機能
	対応プラットフォーム：Windows (C++), Linux (C/C++) ...
******************************************************************************/

/******************************************************************************
include section
******************************************************************************/
#include <stdlib.h>
#include <assert.h>
#include <sharaku/task.h>
#include <sharaku/prof.h>
#include "task-in.h"

////////////////////////////////////////////////////////////////////////
// static変数定義
////////////////////////////////////////////////////////////////////////
static struct sharaku_job_context	_default_context = {
	0,
	0,
	0,
	LIST_HEAD_INIT(_default_context.list),
};
static struct sharaku_job_context	_timer_context = {
	0,
	0,
	0,
	LIST_HEAD_INIT(_timer_context.list),
};
static struct sharaku_job_context	_multi_context = {
	0,
	0,
	0,
	LIST_HEAD_INIT(_multi_context.list),
};
static struct sharaku_job_context*	_contexts[SHARAKU_CONTEXT_MAX];
sharaku_prof_t				_prof_task_timer;
sharaku_prof_t				_prof_task_internal;
sharaku_prof_t				_prof_task_async_msg;
sharaku_prof_t				_prof_task_sync_msg;
sharaku_prof_t				_prof_task_timer_msg;

////////////////////////////////////////////////////////////////////////
// コンテキスト管理I/F
////////////////////////////////////////////////////////////////////////
struct sharaku_job_context* sharaku_get_default_context(void)
{
	return &_default_context;
}

struct sharaku_job_context* sharaku_get_multi_context(void)
{
	return &_multi_context;
}

struct sharaku_job_context* sharaku_get_context(int sid)
{
	struct sharaku_job_context* context = NULL;
	switch (sid) {
	case -1:
		context = &_multi_context;
		break;
	case 0:
		context = &_default_context;
		break;
	default:
		sid --;
		if (sid < SHARAKU_CONTEXT_MAX) {
			context = _contexts[sid];
		}
		break;
	}
	return context;
}

static void _sharaku_initialize_specialcontext(struct sharaku_job_context* context)
{
	context->is_initialize	= 0;
	context->base_time	= 0;
	sharaku_mutex_init(&(context->mx));
	sharaku_cond_init(&(context->cond));
	context->threads	= 0;
	context->is_initialize	= 1;

	return;
}

static void _sharaku_initialize_context(struct sharaku_job_context* context, int threads)
{
	context->is_initialize	= 0;
	context->base_time	= 0;
	INIT_LIST_HEAD(&(context->list));
	sharaku_mutex_init(&(context->mx));
	sharaku_cond_init(&(context->cond));
	context->threads	= threads;
	context->is_initialize	= 1;

	return;
}

static void _sharaku_finalize_context(struct sharaku_job_context* context)
{
	context->is_stop	= 0;
	context->is_initialize	= 0;
	sharaku_mutex_destroy(&(context->mx));

	return;
}

int sharaku_alloc_context(int threads)
{
	int i;
	int salloc_size;

	struct sharaku_job_context* context;

	for (i = 0; i < SHARAKU_CONTEXT_MAX; i++) {
		if (_contexts[i] == NULL) {
			goto next;
		}
	}

	return 0;

 next:
 	salloc_size = sizeof(struct sharaku_job_context)
 			 + sizeof(sharaku_thread_t) * threads;

	context = malloc(salloc_size);
	_sharaku_initialize_context(context, threads);

	_contexts[i] = context;

	// 領域 + 1がIDとなる。
	// defaultが0
	// multiが-1
	return i + 1;

}

void sharaku_free_context(struct sharaku_job_context* context)
{
	_sharaku_finalize_context(context);
	free(context);
}

////////////////////////////////////////////////////////////////////////
// jobスケジューラ全体の初期化/終了処理
////////////////////////////////////////////////////////////////////////
void sharaku_initialize_scheduler(void)
{
	int i;

	// profile準備
	sharaku_prof_init(&_prof_task_timer, "sharaku::task::timer");
	sharaku_prof_init(&_prof_task_internal, "sharaku::task::internal");
	sharaku_prof_init(&_prof_task_async_msg, "sharaku::task::async_message");
	sharaku_prof_init(&_prof_task_sync_msg, "sharaku::task::sync_message");
	sharaku_prof_init(&_prof_task_timer_msg, "sharaku::task::timer_message");
	sharaku_prof_regist(&_prof_task_timer);
	sharaku_prof_regist(&_prof_task_internal);
	sharaku_prof_regist(&_prof_task_async_msg);
	sharaku_prof_regist(&_prof_task_sync_msg);
	sharaku_prof_regist(&_prof_task_timer_msg);

	// 特別スケジューラを初期化
	_sharaku_initialize_specialcontext(&_default_context);
	_sharaku_initialize_specialcontext(&_timer_context);
	_sharaku_initialize_specialcontext(&_multi_context);

	// 一般スケジューラを初期化
	for (i = 0; i < SHARAKU_CONTEXT_MAX; i++) {
		_contexts[i] = NULL;
	}
}

void sharaku_finalize_scheduler(void)
{
	int i;

	// 一般スケジューラを破棄
	for (i = 0; i < SHARAKU_CONTEXT_MAX; i++) {
		if (_contexts[i] != NULL) {
			_sharaku_finalize_context(_contexts[i]);
		}
	}

	// 特別スケジューラを破棄
	_sharaku_finalize_context(&_timer_context);
	_sharaku_finalize_context(&_default_context);
	_sharaku_finalize_context(&_multi_context);
}


////////////////////////////////////////////////////////////////////////
// job初期化、設定処理
////////////////////////////////////////////////////////////////////////

void sharaku_init_job(struct sharaku_job *job)
{
	INIT_LIST_HEAD(&(job->list));
	job->is_timeout = 0;
	job->callback	= NULL;
	job->status	= SHARAKU_JOB_STATUS_STOP;
	job->context	= sharaku_get_default_context();
	job->magic	= SHARAKU_TASK_MAGIC;
}

int sharaku_set_jobparam(struct sharaku_job *job, int sid)
{
	struct sharaku_job_context* context;

	context = sharaku_get_context(sid);
	if (context) {
		job->context = context;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////
// jobスケジュール
////////////////////////////////////////////////////////////////////////

void sharaku_async_message(struct sharaku_job *job,
			    sharaku_job_stagefunc_t callback)
{
	sharaku_usec_t time_start;
	sharaku_usec_t time_end;
	time_start = sharaku_get_usec();

	assert(job->magic == SHARAKU_TASK_MAGIC);

	// Jobの初期化
	job->status	= SHARAKU_JOB_STATUS_QUEUING;
	job->callback 	= callback;

	// スケジューラの末尾へ登録後、待ち合わせているスケジューラを起動する
	// singleスケジューラの場合はこの起動に意味はなく、処理がスケジューラへ
	// 戻された段階で次を処理する
	// 初期化が完了していない場合は排他不要であるため、ルートを分ける
	if (job->context->is_initialize) {
		sharaku_mutex_lock(&(job->context->mx));
		list_add_tail(&(job->list), &(job->context->list));
		sharaku_mutex_unlock(&(job->context->mx));

		sharaku_cond_signal(&(job->context->cond));
	} else {
		list_add_tail(&(job->list), &(job->context->list));
	}

	time_end = sharaku_get_usec();
	sharaku_prof_add(&_prof_task_async_msg, time_start, time_end);
}

void sharaku_sync_message(struct sharaku_job *job,
			   sharaku_job_stagefunc_t callback)
{
	sharaku_usec_t time_start;
	sharaku_usec_t time_end;
	time_start = sharaku_get_usec();

	// Jobの初期化
	job->status	= SHARAKU_JOB_STATUS_QUEUING;
	job->callback 	= callback;
	job->milli_sec	= 0;

	// スケジューラの末尾へ登録後、待ち合わせているスケジューラを起動する
	// singleスケジューラの場合はこの起動に意味はなく、処理がスケジューラへ
	// 戻された段階で次を処理する
	// 初期化が完了していない場合は排他不要であるため、ルートを分ける
	if (job->context->is_initialize) {
		sharaku_mutex_lock(&(job->context->mx));
		list_add_tail(&(job->list), &(job->context->list));
		sharaku_mutex_unlock(&(job->context->mx));

		sharaku_cond_signal(&(job->context->cond));
	} else {
		list_add_tail(&(job->list), &(job->context->list));
	}

	time_end = sharaku_get_usec();
	sharaku_prof_add(&_prof_task_sync_msg, time_start, time_end);
}

void sharaku_timer_message(struct sharaku_job *job,
			    uint32_t millisecond,
			    sharaku_job_stagefunc_t callback)
{
	sharaku_usec_t time_start;
	sharaku_usec_t time_end;
	time_start = sharaku_get_usec();

	// Jobの初期化
	job->status	= SHARAKU_JOB_STATUS_SUSPEND;
	job->callback	= callback;
	job->milli_sec	= _timer_context.base_time + millisecond;

	// スケジューラへ登録
	// 初期化が完了していない場合は排他不要であるため、ルートを分ける
	if (_timer_context.is_initialize) {
		sharaku_mutex_lock(&(_timer_context.mx));
		list_add_tail(&(job->list), &(_timer_context.list));
		sharaku_mutex_unlock(&(_timer_context.mx));
	} else {
		list_add_tail(&(job->list), &(_timer_context.list));
	}

	time_end = sharaku_get_usec();
	sharaku_prof_add(&_prof_task_timer_msg, time_start, time_end);
}

void sharaku_cancel_message(struct sharaku_job *job)
{
	// スケジューラからjobを削除する。
	// 起動中でない限り、この操作で削除可能
	if (SHARAKU_JOB_STATUS_SUSPEND == job->status) {
		// Timerスケジュール中
		sharaku_mutex_lock(&(_timer_context.mx));
		job->status = SHARAKU_JOB_STATUS_STOP;
		list_del_init(&(job->list));
		sharaku_mutex_unlock(&(_timer_context.mx));
	} else {
		// 一般スケジューラ
		sharaku_mutex_lock(&(job->context->mx));
		job->status = SHARAKU_JOB_STATUS_STOP;
		list_del_init(&(job->list));
		sharaku_mutex_unlock(&(job->context->mx));
	}
}

////////////////////////////////////////////////////////////////////////
// jobスケジューラ エントリー処理
////////////////////////////////////////////////////////////////////////

// jobのサイクル処理
void sharaku_entry_job_cycle(struct sharaku_job_context* context)
{
	struct sharaku_job	*job;
	sharaku_usec_t time_start;
	sharaku_usec_t time_end;
	time_start = sharaku_get_usec();

	sharaku_mutex_lock(&(context->mx));
//	// jobがなければ登録されるまで待つ
//	// 待ち解除条件がアプリ終了指示である場合は停止する。
//	if(list_empty(&(context->list))) {
//		sharaku_cond_wait(&(context->cond), &(context->mx));
//		sharaku_mutex_unlock(&(context->mx));
//		return;
//	}

	// 先頭を取り出す
	job = list_entry(context->list.next, struct sharaku_job, list);
	list_del_init(&(job->list));
	sharaku_mutex_unlock(&(context->mx));
	job->status	= SHARAKU_JOB_STATUS_ACTIVE;

	// 登録されているCallBackを呼び出す
	if (job->callback) {
		job->callback(job);
	}

	if (SHARAKU_JOB_STATUS_ACTIVE == job->status) {
		// ACTIVEのまま終了したものは停止状態にする
		job->status = SHARAKU_JOB_STATUS_STOP;
	}

	time_end = sharaku_get_usec();
	sharaku_prof_add(&_prof_task_internal, time_start, time_end);
}

void sharaku_entry_job_timer(uint32_t millisecond)
{
	struct sharaku_job	*job;
	struct sharaku_job	*n;
	struct list_head	list;
	sharaku_usec_t time_start;
	sharaku_usec_t time_end;
	time_start = sharaku_get_usec();

	INIT_LIST_HEAD(&list);

	_timer_context.base_time += millisecond;

	if (!_timer_context.is_initialize) {
		return;
	}

	// タイムアウトしたjobのリストを作成する
	sharaku_mutex_lock(&(_timer_context.mx));
	list_for_each_entry_safe(job, n, &(_timer_context.list),
				 struct sharaku_job, list) {

		// 時間経過していたらmessageをjob実行キューの末尾につなぐ
		// job_cond_signalを行うが、排他を解除しない限り次は起動しない。
		if(job->milli_sec > _timer_context.base_time) {
			continue;
		}
		list_move_tail(&(job->list), &list);
	}
	sharaku_mutex_unlock(&(_timer_context.mx));

	list_for_each_entry_safe(job, n, &list, struct sharaku_job, list) {
		list_del_init(&(job->list));
		job->status = SHARAKU_JOB_STATUS_STOP;

		sharaku_mutex_lock(&(job->context->mx));
		list_add_tail(&(job->list), &(job->context->list));
		sharaku_mutex_unlock(&(job->context->mx));
	}

	time_end = sharaku_get_usec();
	sharaku_prof_add(&_prof_task_timer, time_start, time_end);
}
