/* --
MIT License

Copyright (c) 2004-2017 Abe Takafumi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**/

#include <stdlib.h>
#include <assert.h>
#include <sharaku/sched.h>
#include <sharaku/plist.h>
#include <sharaku/prof.h>
#include <sharaku/jiffies.h>

//#define SCHED_PROFILE_ENABLE

static struct sched_context _default_context
	= SHARAKU_CONTEXT_INIT(_default_context);

#ifdef SCHED_PROFILE_ENABLE
static sharaku_prof_t	_prof_task_timer
	 = SHARAKU_PROF_INIT(_prof_task_timer,
	 			"sharaku::task::timer");
static sharaku_prof_t	_prof_task_internal;
	 = SHARAKU_PROF_INIT(_prof_task_internal,
	 			"sharaku::task::internal");
static sharaku_prof_t	_prof_task_async_msg;
	 = SHARAKU_PROF_INIT(_prof_task_async_msg,
	 			"sharaku::task::async_message");
static sharaku_prof_t	_prof_task_sync_msg;
	 = SHARAKU_PROF_INIT(_prof_task_sync_msg,
	 			"sharaku::task::sync_message");
static sharaku_prof_t	_prof_task_timer_msg;
	 = SHARAKU_PROF_INIT(_prof_task_timer_msg,
	 			"sharaku::task::timer_message");
#endif

static inline int
_is_initialize(struct sched_context* context)
{
	return context->is_initialize;
}

static inline void
_context_init(struct sched_context* context)
{
	context->is_initialize	= 0;
	context->is_stop	= 0;
	context->rsv		= 0;
	context->base_time	= 0;
	sharaku_mutex_init(&(context->plist_mx));
	sharaku_mutex_init(&(context->timer_plist_mx));
	context->is_initialize	= 1;
	return;
}

static inline void
_context_finl(struct sched_context* context)
{
	context->is_stop	= 1;
	context->is_initialize	= 0;
	sharaku_mutex_destroy(&(context->plist_mx));
	sharaku_mutex_destroy(&(context->timer_plist_mx));
	return;
}

static inline void
_push_sched(job_t *job, struct sched_context *context)
{
	// 初期化前の場合はミューテックスをかけられないため、排他せずに
	// 登録する。
	job->node.prio = job->prio;
	if (_is_initialize(context)) {
		sharaku_mutex_lock(&context->plist_mx);
		plist_add(&(job->node), &(context->plist));
		sharaku_mutex_unlock(&context->plist_mx);
	} else {
		plist_add(&(job->node), &(context->plist));
	}
}

static inline void
_push_timer(job_t *job, struct sched_context *context)
{
	// 初期化前の場合はミューテックスをかけられないため、排他せずに
	// 登録する。
	job->node.prio = job->milli_sec;
	if (_is_initialize(context)) {
		sharaku_mutex_lock(&context->timer_plist_mx);
		plist_add(&(job->node), &(context->timer_plist));
		sharaku_mutex_unlock(&context->timer_plist_mx);
	} else {
		plist_add(&(job->node), &(context->timer_plist));
	}
}

void
sharaku_init_sched(void)
{
	_context_init(&_default_context);
}

void
sharaku_finl_sched(void)
{
	_context_finl(&_default_context);
}

void
init_job_prio(job_t *job, int prio)
{
	INIT_PLIST_NODE(&(job->node), prio);
	job->callback	= NULL;
	job->prio	= prio;
	job->milli_sec	= 0;
	job->context	= &_default_context;
	job->status	= SHARAKU_JOB_STATUS_SUSPEND;
	job->is_timeout = 0;
	job->magic	= SHARAKU_TASK_MAGIC;
}

void
init_job(job_t *job)
{
	init_job_prio(job, SHARAKU_DEFAULT_PRIO);
}

void
job_async_sched(job_t *job, job_stagefunc_t cb)
{
#ifdef SCHED_PROFILE_ENABLE
	sharaku_usec_t time_start;
	sharaku_usec_t time_end;
	time_start = sharaku_get_usec();
#endif

	// Jobの初期化
	job->status	= SHARAKU_JOB_STATUS_READY;
	job->callback 	= cb;
	job->milli_sec	= 0;

	// スケジューラの末尾へ登録後、待ち合わせているスケジューラを起動する
	// singleスケジューラの場合はこの起動に意味はなく、処理がスケジューラへ
	// 戻された段階で次を処理する
	_push_sched(job, job->context);


#ifdef SCHED_PROFILE_ENABLE
	time_end = sharaku_get_usec();
	sharaku_prof_add(&_prof_task_async_msg, time_start, time_end);
#endif
}

void
job_timer_sched(job_t *job, uint32_t ms, job_stagefunc_t cb)
{
#ifdef SCHED_PROFILE_ENABLE
	sharaku_usec_t time_start;
	sharaku_usec_t time_end;
	time_start = sharaku_get_usec();
#endif

	// Jobの初期化
	job->status	= SHARAKU_JOB_STATUS_WAIT;
	job->callback	= cb;
	job->milli_sec	= sharaku_get_msec() + ms;

	// スケジューラへ登録
	// 初期化が完了していない場合は排他不要であるため、ルートを分ける
	_push_timer(job, job->context);

#ifdef SCHED_PROFILE_ENABLE
	time_end = sharaku_get_usec();
	sharaku_prof_add(&_prof_task_timer_msg, time_start, time_end);
#endif
}

void
job_cancel_sched(job_t *job)
{
	struct sched_context *context = job->context;
	// スケジューラからjobを削除する。
	// 起動中でない限り、この操作で削除可能
	job->status = SHARAKU_JOB_STATUS_SUSPEND;
	if (SHARAKU_JOB_STATUS_WAIT == job->status) {
		// Timerスケジュール中
		if (_is_initialize(context)) {
			sharaku_mutex_lock(&context->timer_plist_mx);
			plist_del(&job->node, &(context->timer_plist));
			sharaku_mutex_unlock(&context->timer_plist_mx);
		} else {
			plist_del(&job->node, &(context->timer_plist));
		}
	} else {
		// 一般スケジューラ
		if (_is_initialize(context)) {
			sharaku_mutex_lock(&context->plist_mx);
			plist_del(&job->node, &(context->plist));
			sharaku_mutex_unlock(&context->plist_mx);
		} else {
			plist_del(&job->node, &(context->plist));
		}
	}
}

static inline void
_do_sched_timer(struct sched_context* context)
{
	job_t	*job;
	job_t	*n;
	struct list_head	list;
#ifdef SCHED_PROFILE_ENABLE
	sharaku_usec_t time_start;
	sharaku_usec_t time_end;
#endif

	// 初期化中はreturnする。
	if (!_is_initialize(context)) {
		goto out;
	}

	// jiffiseをmsへ変換したものを代入する。
	// jiffiseを使うことで無駄なシステムコール呼び出しをなくす。
	if (context->base_time == sharaku_get_msec()) {
		goto out;
	}
	context->base_time = sharaku_get_msec();

#ifdef SCHED_PROFILE_ENABLE
	time_start = sharaku_get_usec();
#endif
	INIT_LIST_HEAD(&list);

	// タイムアウトしたjobのリストを作成する
	// タイマーのリストは昇順に並んでいる。
	sharaku_mutex_lock(&context->timer_plist_mx);
	plist_for_each_entry_safe(job, n, &context->timer_plist,
					 job_t, node) {
		if (job->milli_sec > context->base_time) {
			break;
		}
		job->status = SHARAKU_JOB_STATUS_SUSPEND;
		plist_del(&(job->node), &(context->timer_plist));
		list_add_tail(&job->node.node_list, &list);
	}
	sharaku_mutex_unlock(&context->timer_plist_mx);

	// jobごとにスケジューラが異なるかもしれないため、1jobずつ
	// スケジュールする
	list_for_each_entry_safe(job, n, &list, job_t, node.node_list) {
		list_del_init(&(job->node.node_list));
		job_async_sched(job, job->callback);
	}

#ifdef SCHED_PROFILE_ENABLE
	time_end = sharaku_get_usec();
	sharaku_prof_add(&_prof_task_timer, time_start, time_end);
#endif
out:
	return;
}

// jobのサイクル処理
static inline void
_do_sched(struct sched_context* context)
{
	job_t	*job;
#ifdef SCHED_PROFILE_ENABLE
	sharaku_usec_t time_start;
	sharaku_usec_t time_end;
	time_start = sharaku_get_usec();
#endif

	sharaku_mutex_lock(&context->plist_mx);
	if (list_empty(&(context->plist.node_list))) {
		// 実行すべきjobがない
		sharaku_mutex_unlock(&(context->plist_mx));
		return;
	}
	// 先頭を取り出す
	job = list_entry(context->plist.node_list.next,
				job_t, node.node_list);
	plist_del(&(job->node), &(context->plist));
	sharaku_mutex_unlock(&(context->plist_mx));

	// 登録されているCallBackを呼び出す
	job->status = SHARAKU_JOB_STATUS_RUN;
	job->callback(job);

	// ACTIVEのまま終了したものは停止状態にする
	// callbackの中でキューイングしたものはSHARAKU_JOB_STATUS_QUEUING状態
	// となる。
	if (SHARAKU_JOB_STATUS_RUN == job->status) {
		job->status = SHARAKU_JOB_STATUS_SUSPEND;
	}

#ifdef SCHED_PROFILE_ENABLE
	time_end = sharaku_get_usec();
	sharaku_prof_add(&_prof_task_internal, time_start, time_end);
#endif
}

void
sharaku_workqueue(void)
{
	_do_sched_timer(&_default_context);
	_do_sched(&_default_context);
}
