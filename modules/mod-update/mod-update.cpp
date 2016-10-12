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
#include <sharaku/module.h>
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
void
update_group::register_update(update_operations* update)
{
	sharaku_db_trace("", 0, 0, 0, 0, 0, 0);
	list_add(&update->update_list, &_update_list);
}
int32_t
update_group::pre_update(const float &interval, uint32_t retry_cnt)
{
	update_operations	*update;

	sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);
	list_for_each_entry(update, &_update_list, update_operations, update_list) {
		update->pre_update(interval, retry_cnt);
		sharaku_db_trace("update->pre_update", 0, 0, 0, 0, 0, 0);
	}
	sharaku_db_trace("end", 0, 0, 0, 0, 0, 0);

	return 0;
}

int32_t
update_group::update(const float &interval, uint32_t retry_cnt)
{
	update_operations	*update;
	sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);
	list_for_each_entry(update, &_update_list, update_operations, update_list) {
		update->update(interval, retry_cnt);
		sharaku_db_trace("update->update", 0, 0, 0, 0, 0, 0);
	}
	sharaku_db_trace("end", 0, 0, 0, 0, 0, 0);

	return 0;
}

int32_t
update_group::post_update(const float &interval, uint32_t retry_cnt)
{
	update_operations	*update;
	sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);
	list_for_each_entry(update, &_update_list, update_operations, update_list) {
		update->post_update(interval, retry_cnt);
		sharaku_db_trace("update->post_update", 0, 0, 0, 0, 0, 0);
	}
	sharaku_db_trace("end", 0, 0, 0, 0, 0, 0);

	return 0;
}


/******************************************************************************
mod_update
******************************************************************************/
mod_update::mod_update(uint32_t interval)
{
	_time			= 0;
	_update_count		= 0;
	_update_start_time	= 0;
	_update_time		= 0;
	_time			= 0;
	_interval_ms		= interval;
	_is_stop		= 1;
	sharaku_init_job(&_job_update);
	INIT_LIST_HEAD(&_cycle_function);
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
	INIT_LIST_HEAD(&_cycle_function);
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
	sharaku_prof_regist(&__prof_update_processing);
	sharaku_prof_regist(&__prof_update_pre_update_processing);
	sharaku_prof_regist(&__prof_update_update_processing);
	sharaku_prof_regist(&__prof_update_post_update_processing);
}

void
mod_update::mod_update_cycle(struct sharaku_job* job)
{
	sharaku_usec_t		time_cycle_start;
	sharaku_usec_t		time_pre_update_end;
	sharaku_usec_t		time_update_end;
	sharaku_usec_t		time_post_update_end;
	mod_update		*device;
	device = (mod_update*)list_entry(job, mod_update, _job_update);

	sharaku_usec_t time = sharaku_get_usec();
	// 停止指示がある場合は停止する
	if (device->_is_stop) {
		return;
	}

	sharaku_db_trace("update interval=%u", (time - device->_update_time), 0, 0, 0, 0, 0);

	// スケジュールする。
	// 最初にスケジュールすることで誤差を少なくする
	// 本関数が終了するまでここでスケジュールした関数は起動しないため
	// このタイミングで起動しても問題ない
	sharaku_timer_message(job, device->_interval_ms, mod_update_cycle);

	time_cycle_start = sharaku_get_usec();
	// 時間収集(最初の1回は採取しない)
	if (device->_update_time) {
		sharaku_prof_add(&__prof_update_interval,
					 device->_update_time, time_cycle_start);
	}
	device->_update_time = time_cycle_start;

	// 平均インターバルを計算する
	device->_update_count ++;
	float interval	= (float)(time_cycle_start - device->_update_start_time)
					/ (float)device->_update_count / 1000000.0f;

	// pre_updateを実行する
	device->pre_update(interval, 0);
	time_pre_update_end = sharaku_get_usec();
	sharaku_prof_add(&__prof_update_pre_update_processing,
					time_cycle_start, time_pre_update_end);

	// updateを実行する
	device->update(interval, 0);
	time_update_end = sharaku_get_usec();
	sharaku_prof_add(&__prof_update_update_processing,
					time_pre_update_end, time_update_end);

	// post_updateを実行する
	device->post_update(interval, 0);
	time_post_update_end = sharaku_get_usec();
	sharaku_prof_add(&__prof_update_post_update_processing,
					time_update_end, time_post_update_end);
	sharaku_prof_add(&__prof_update_processing,
					time_cycle_start, time_post_update_end);

	sharaku_db_trace("time=%d", (int32_t)(sharaku_get_usec() - time), 0, 0, 0, 0, 0);
}

SHARAKU_REGIST_MODULE_BEGIN(UPDATE_MODULE_ID)
  init = mod_update_init;
SHARAKU_REGIST_MODULE_END

NAMESPACE_SHARAKU_END
