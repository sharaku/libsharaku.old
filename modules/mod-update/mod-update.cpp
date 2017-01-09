/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2015
 * Author Abe Takafumi
 *
 */

#include <math.h>
#include <stdio.h>
#include <sharaku/types.h>
#include <sharaku/prof.h>
#include <sharaku/log.h>
#include <sharaku/list.h>
#include <sharaku/debug.h>
#include <modules/mod-update.hpp>

#define UPDATE_MODULE_ID	0

NAMESPACE_SHARAKU_BEGIN

/******************************************************************************
definition section
******************************************************************************/
sharaku_prof_t	__prof_update_interval;
sharaku_prof_t	__prof_update_processing;
sharaku_prof_t	__prof_update_pre_update_processing;
sharaku_prof_t	__prof_update_update_processing;
sharaku_prof_t	__prof_update_post_update_processing;

/******************************************************************************
class section
******************************************************************************/

/******************************************************************************
mod_update
******************************************************************************/
mod_update::mod_update(uint32_t interval)
{
	_job_update_i = &_job_update_interval;

	_time			= 0;
	_update_count		= 0;
	_update_start_time	= 0;
	_update_time		= 0;
	_time			= 0;
	_interval_ms		= interval;
	_is_stop		= 1;
	sharaku_init_job(&_job_update);
	sharaku_init_job(&_job_update_interval);
	INIT_LIST_HEAD(&_update_list);
}
mod_update::mod_update() {
	_time			= 0;
	_update_count		= 0;
	_update_start_time	= 0;
	_update_time		= 0;
	_time			= 0;
	_interval_ms		= 0;
	_is_stop		= 1;
	sharaku_init_job(&_job_update);
	INIT_LIST_HEAD(&_update_list);
}

void
mod_update::register_update(update_operations* update)
{
	sharaku_db_trace("", 0, 0, 0, 0, 0, 0);
	list_add(&update->update_list, &_update_list);
}

void
mod_update::start()
{
	if (_is_stop) {
		_update_count		= 0;
		_update_start_time	= sharaku_get_usec();

		sharaku_init_job(&_job_update);
		sharaku_async_message(&_job_update, mod_update::mod_update_cycle);
		_is_stop = 0;
	}
}

void
mod_update::stop()
{
	if (!_is_stop) {
		_is_stop = 1;
		sharaku_cancel_message(&_job_update);
	}
}

uint32_t
mod_update::get_prof_count_interval()
{
	return sharaku_prof_get_count(&__prof_update_interval);
}

sharaku_usec_t
mod_update::get_prof_usec_interval()
{
	return sharaku_prof_get_usec(&__prof_update_interval);
}

uint32_t
mod_update::get_prof_count_processing()
{
	return sharaku_prof_get_count(&__prof_update_processing);
}

sharaku_usec_t
mod_update::get_prof_usec_processing()
{
	return sharaku_prof_get_usec(&__prof_update_processing);
}

/*
 * module
 */
static void
mod_update_init(void)
{
	sharaku_prof_init(&__prof_update_interval, "mod_update::interval");
	sharaku_prof_init(&__prof_update_processing, "mod_update::processing");
	sharaku_prof_init(&__prof_update_pre_update_processing, "mod_update::pre_update_processing");
	sharaku_prof_init(&__prof_update_update_processing, "mod_update::update_processing");
	sharaku_prof_init(&__prof_update_post_update_processing, "mod_update::post_update_processing");
	sharaku_prof_regist(&__prof_update_interval);
	debug_code(sharaku_prof_regist(&__prof_update_processing));
	debug_code(sharaku_prof_regist(&__prof_update_pre_update_processing));
	debug_code(sharaku_prof_regist(&__prof_update_update_processing));
	debug_code(sharaku_prof_regist(&__prof_update_post_update_processing));
}

// -------------------------------------------------------------------------
void
mod_update::mod_update_cycle(struct sharaku_job* job)
{
	mod_update *_mod_update = (mod_update*)list_entry(job, mod_update, _job_update);
	uint32_t interval = 0;

	// 前のスケジュールが完了していない場合は、非同期で本関数を呼び出す。
	if (!_mod_update->_job_update_i) {
		sharaku_db_trace("update busy", 0, 0, 0, 0, 0, 0);
		sharaku_async_message(job, mod_update_cycle);
		return;
	}

	_mod_update->_update_cycle_start = sharaku_get_usec();
	// 時間収集(最初の1回は採取しない)
	if (_mod_update->_update_time) {
		sharaku_prof_add(&__prof_update_interval,
				 _mod_update->_update_time,
				 _mod_update->_update_cycle_start);
		interval = _mod_update->_update_cycle_start - _mod_update->_update_time;
	}
	_mod_update->_update_time = _mod_update->_update_cycle_start;

	// 平均インターバルを計算する
	_mod_update->_update_count ++;
	_mod_update->_interval	= (float)(_mod_update->_update_cycle_start - _mod_update->_update_start_time)
					/ (float)_mod_update->_update_count / 1000000.0f;


	sharaku_usec_t time = sharaku_get_usec();
	// 停止指示がある場合は停止する
	if (_mod_update->_is_stop) {
		return;
	}

	sharaku_db_trace("update start: update interval=%dus, avg interval=%dus",
			 (uint32_t)interval, (uint32_t)(_mod_update->_interval * 1000000), 0, 0, 0, 0);

	// スケジュールする。
	// 最初にスケジュールすることで誤差を少なくする
	// 本関数が終了するまでここでスケジュールした関数は起動しないため
	// このタイミングで起動しても問題ない
	sharaku_timer_message(job, _mod_update->_interval_ms, mod_update_cycle);
	_mod_update->_job_update_i = NULL;
	mod_pre_update_begin(&_mod_update->_job_update_interval);
}

// -------------------------------------------------------------------------
// pre_updateを処理する
void
mod_update::mod_pre_update_begin(struct sharaku_job* job)
{
	sharaku_db_trace("", 0, 0, 0, 0, 0, 0);
	mod_update *_mod_update = (mod_update*)list_entry(job, mod_update, _job_update_interval);
	_mod_update->_time_start = sharaku_get_usec();
	_mod_update->_update = list_first_entry(&_mod_update->_update_list,
						update_operations, update_list);
	mod_pre_update(job);
}

void
mod_update::mod_pre_update(struct sharaku_job* job)
{
	mod_update *_mod_update = (mod_update*)list_entry(job, mod_update, _job_update_interval);
	int32_t	rc = 0;

next:
	// 停止指示がある場合は停止する
	if (_mod_update->_is_stop) {
		return;
	}
	// 最後まで完了している場合は、Endへ行く
	if (&_mod_update->_update->update_list == &_mod_update->_update_list) {
		mod_pre_update_end(job);
		return;
	}

	// pre_updateを実行する
	// EAGAINを返された場合は、いま処理したupdateをもう一度実行する。
	sharaku_usec_t time_start = sharaku_get_usec();
	if (_mod_update->_update->_prof_time_per_update_start) {
		sharaku_prof_add(&_mod_update->_update->_prof_interval, _mod_update->_update->_prof_time_per_update_start, time_start);
	}

	_mod_update->_update->_prof_time_per_update_start = time_start;
	rc = _mod_update->_update->pre_update(_mod_update->_interval, 0);
	if (rc != EAGAIN) {
		sharaku_usec_t time_process = sharaku_get_usec();
		sharaku_prof_add(&_mod_update->_update->_prof_pre_update_process, _mod_update->_update->_prof_time_per_update_start, time_process);
		_mod_update->_update = list_next_entry(_mod_update->_update,
							update_operations, update_list);
		goto next;
	} else {
		sharaku_prof_add_notime(&_mod_update->_update->_prof_pre_update_retry);
		sharaku_async_message(job, mod_pre_update_retry);
		return;
	}
}

void
mod_update::mod_pre_update_retry(struct sharaku_job* job)
{
	mod_update *_mod_update = (mod_update*)list_entry(job, mod_update, _job_update_interval);
	int32_t	rc = 0;

	// 停止指示がある場合は停止する
	if (_mod_update->_is_stop) {
		return;
	}
	// pre_updateを実行する
	// EAGAINを返された場合は、いま処理したupdateをもう一度実行する。
	rc = _mod_update->_update->pre_update(_mod_update->_interval, 1);
	if (rc != EAGAIN) {
		sharaku_usec_t time_process = sharaku_get_usec();
		sharaku_prof_add(&_mod_update->_update->_prof_pre_update_process, _mod_update->_update->_prof_time_per_update_start, time_process);
		_mod_update->_update = list_next_entry(_mod_update->_update,
							update_operations, update_list);
		sharaku_async_message(job, mod_pre_update);
		return;
	} else {
		sharaku_prof_add_notime(&_mod_update->_update->_prof_pre_update_retry);
		sharaku_async_message(job, mod_pre_update_retry);
		return;
	}
}

void
mod_update::mod_pre_update_end(struct sharaku_job* job)
{
	sharaku_db_trace("", 0, 0, 0, 0, 0, 0);
	mod_update *_mod_update = (mod_update*)list_entry(job, mod_update, _job_update_interval);
	// 統計情報を取って次へ行く
	sharaku_usec_t	time_update_end = sharaku_get_usec();
	sharaku_prof_add(&__prof_update_pre_update_processing,
			 _mod_update->_time_start, time_update_end);
	mod_update_begin(job);
}

// -------------------------------------------------------------------------
// updateを処理する
void
mod_update::mod_update_begin(struct sharaku_job* job)
{
	sharaku_db_trace("", 0, 0, 0, 0, 0, 0);
	mod_update *_mod_update = (mod_update*)list_entry(job, mod_update, _job_update_interval);
	_mod_update->_time_start = sharaku_get_usec();
	_mod_update->_update = list_first_entry(&_mod_update->_update_list,
						update_operations, update_list);
	mod_update_(job);
}

void
mod_update::mod_update_(struct sharaku_job* job)
{
	mod_update *_mod_update = (mod_update*)list_entry(job, mod_update, _job_update_interval);
	int32_t	rc = 0;

next:
	// 停止指示がある場合は停止する
	if (_mod_update->_is_stop) {
		return;
	}
	// 最後まで完了している場合は、Endへ行く
	if (&_mod_update->_update->update_list == &_mod_update->_update_list) {
		mod_update_end(job);
		return;
	}

	// updateを実行する
	_mod_update->_update->_prof_time_update_start =  sharaku_get_usec();
	rc = _mod_update->_update->update(_mod_update->_interval, 0);
	if (rc != EAGAIN) {
		sharaku_usec_t time_process = sharaku_get_usec();
		sharaku_prof_add(&_mod_update->_update->_prof_update_process, _mod_update->_update->_prof_time_update_start, time_process);
		_mod_update->_update = list_next_entry(_mod_update->_update,
							update_operations, update_list);
		goto next;
	} else {
		sharaku_prof_add_notime(&_mod_update->_update->_prof_update_retry);
		sharaku_async_message(job, mod_update_retry);
		return;
	}
}

void
mod_update::mod_update_retry(struct sharaku_job* job)
{
	mod_update *_mod_update = (mod_update*)list_entry(job, mod_update, _job_update_interval);
	int32_t	rc = 0;

	// 停止指示がある場合は停止する
	if (_mod_update->_is_stop) {
		return;
	}
	// updateを実行する
	rc = _mod_update->_update->update(_mod_update->_interval, 1);
	if (rc != EAGAIN) {
		sharaku_usec_t time_process = sharaku_get_usec();
		sharaku_prof_add(&_mod_update->_update->_prof_update_process, _mod_update->_update->_prof_time_update_start, time_process);
		_mod_update->_update = list_next_entry(_mod_update->_update,
							update_operations, update_list);
		sharaku_async_message(job, mod_update_);
	} else {
		sharaku_prof_add_notime(&_mod_update->_update->_prof_update_retry);
		sharaku_async_message(job, mod_update_retry);
	}
}

void
mod_update::mod_update_end(struct sharaku_job* job)
{
	sharaku_db_trace("", 0, 0, 0, 0, 0, 0);
	mod_update *_mod_update = (mod_update*)list_entry(job, mod_update, _job_update_interval);
	// 統計情報を取って次へ行く
	sharaku_usec_t	time_update_end = sharaku_get_usec();
	sharaku_prof_add(&__prof_update_update_processing,
			 _mod_update->_time_start, time_update_end);
	mod_post_update_begin(job);
}

// -------------------------------------------------------------------------
// post_updateを処理する
void
mod_update::mod_post_update_begin(struct sharaku_job* job)
{
	sharaku_db_trace("", 0, 0, 0, 0, 0, 0);
	mod_update *_mod_update = (mod_update*)list_entry(job, mod_update, _job_update_interval);
	_mod_update->_time_start = sharaku_get_usec();
	_mod_update->_update = list_last_entry(&_mod_update->_update_list,
						update_operations, update_list);
	mod_post_update(job);
}

void
mod_update::mod_post_update(struct sharaku_job* job)
{
	mod_update *_mod_update = (mod_update*)list_entry(job, mod_update, _job_update_interval);
	int32_t	rc = 0;

next:
	// 停止指示がある場合は停止する
	if (_mod_update->_is_stop) {
		return;
	}
	// 最後まで完了している場合は、Endへ行く
	if (&_mod_update->_update->update_list == &_mod_update->_update_list) {
		mod_post_update_end(job);
		return;
	}

	// post_updateを実行する
	// EAGAINを返された場合は、いま処理したupdateをもう一度実行する。
	_mod_update->_update->_prof_time_post_update_start =  sharaku_get_usec();
	rc = _mod_update->_update->post_update(_mod_update->_interval, 0);
	if (rc != EAGAIN) {
		sharaku_usec_t time_process = sharaku_get_usec();
		sharaku_prof_add(&_mod_update->_update->_prof_post_update_process, _mod_update->_update->_prof_time_post_update_start, time_process);
		_mod_update->_update = list_prev_entry(_mod_update->_update,
							update_operations, update_list);
		goto next;
	} else {
		sharaku_prof_add_notime(&_mod_update->_update->_prof_post_update_retry);
		sharaku_async_message(job, mod_post_update_retry);
		return;
	}
}

void
mod_update::mod_post_update_retry(struct sharaku_job* job)
{
	mod_update *_mod_update = (mod_update*)list_entry(job, mod_update, _job_update_interval);
	int32_t	rc = 0;

	// 停止指示がある場合は停止する
	if (_mod_update->_is_stop) {
		return;
	}
	// post_updateを実行する
	// EAGAINを返された場合は、いま処理したupdateをもう一度実行する。
	rc = _mod_update->_update->post_update(_mod_update->_interval, 1);
	if (rc != EAGAIN) {
		sharaku_usec_t time_process = sharaku_get_usec();
		sharaku_prof_add(&_mod_update->_update->_prof_post_update_process, _mod_update->_update->_prof_time_post_update_start, time_process);
		_mod_update->_update = list_prev_entry(_mod_update->_update,
							update_operations, update_list);
		sharaku_async_message(job, mod_post_update);
		return;
	} else {
		sharaku_prof_add_notime(&_mod_update->_update->_prof_post_update_retry);
		sharaku_async_message(job, mod_post_update_retry);
		return;
	}
}

void
mod_update::mod_post_update_end(struct sharaku_job* job)
{
	sharaku_db_trace("", 0, 0, 0, 0, 0, 0);
	mod_update *_mod_update = (mod_update*)list_entry(job, mod_update, _job_update_interval);
	// 統計情報を取って次へ行く
	sharaku_usec_t	time_update_end = sharaku_get_usec();
	sharaku_prof_add(&__prof_update_pre_update_processing,
			 _mod_update->_time_start, time_update_end);
	sharaku_prof_add(&__prof_update_processing,
			 _mod_update->_update_cycle_start, time_update_end);

	// 使用していたjobを返却する
	// - mod_update_cycleがサイクルを経過済みの場合は、次のasync_message
	//   起動を契機にmod_update_cycleが次をキックする。
	// - mod_update_cycleがサイクルを経過していない場合は、次のサイクル
	//   起動を契機にmod_update_cycleが次をキックする。
	_mod_update->_job_update_i = job;
}

NAMESPACE_SHARAKU_END
