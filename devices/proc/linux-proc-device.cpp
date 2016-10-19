/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#include <stdio.h>
#include <string.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <sharaku/types.h>
#include <devices/proc/linux-proc-device.hpp>

NAMESPACE_SHARAKU_BEGIN

// 定期的にprocを読み込んで更新する
void
linux_proc_device::update_device(struct sharaku_job *job)
{
	linux_proc_device *_this = list_entry(job, linux_proc_device, _job_interval);
	if (_this->_interval_ms) {
		// intervalが0以外の場合は、スケジュールする。
		sharaku_timer_message(job,
				      _this->_interval_ms,
				      linux_proc_device::update_device);
	} else {
		sharaku_mutex_lock(&_this->_mutex_job_i);
		// _job_intervalの使用が完了したら返却する
		_this->_job_i = &_this->_job_interval;
		sharaku_mutex_unlock(&_this->_mutex_job_i);
	}

	// updateの必要があればupdateする
	if (_this->_job) {
		_this->_job = NULL;

		sharaku_usec_t time_start = sharaku_get_usec();
		if (_this->_prof_time_start) {
			sharaku_prof_add(&_this->_prof_read_interval, time_start, _this->_prof_time_start);
			sharaku_prof_add(&_this->_prof_write_interval, time_start, _this->_prof_time_start);
		}
		_this->_prof_time_start = time_start;

		_this->__update();
		_this->__commit();
		io_submit(&_this->_job_update);
	}
}
void
linux_proc_device::start_update_device(struct sharaku_job *job)
{
	linux_proc_device *_this = list_entry(job, linux_proc_device, _job_interval);
	if (_this->_interval_ms) {
		// intervalが0以外の場合は、スケジュールする。
		sharaku_timer_message(job,
				      _this->_interval_ms,
				      linux_proc_device::update_device);
	} else {
		sharaku_mutex_lock(&_this->_mutex_job_i);
		// _job_intervalの使用が完了したら返却する
		_this->_job_i = &_this->_job_interval;
		sharaku_mutex_unlock(&_this->_mutex_job_i);
	}

	// updateの必要があればupdateする
	if (_this->_job) {
		_this->_job = NULL;

		sharaku_usec_t time_start = sharaku_get_usec();
		if (_this->_prof_time_start) {
			sharaku_prof_add(&_this->_prof_read_interval, time_start, _this->_prof_time_start);
		}
		_this->_prof_time_start = time_start;

		_this->__update();
		io_submit(&_this->_job_update);
	}
}
void
linux_proc_device::start_commit_device(struct sharaku_job *job)
{
	linux_proc_device *_this = list_entry(job, linux_proc_device, _job_interval);
	if (_this->_interval_ms) {
		// intervalが0以外の場合は、スケジュールする。
		sharaku_timer_message(job,
				      _this->_interval_ms,
				      linux_proc_device::update_device);
	} else {
		sharaku_mutex_lock(&_this->_mutex_job_i);
		// _job_intervalの使用が完了したら返却する
		_this->_job_i = &_this->_job_interval;
		sharaku_mutex_unlock(&_this->_mutex_job_i);
	}

	// updateの必要があればupdateする
	if (_this->_job) {
		_this->_job = NULL;

		sharaku_usec_t time_start = sharaku_get_usec();
		if (_this->_prof_time_start) {
			sharaku_prof_add(&_this->_prof_write_interval, time_start, _this->_prof_time_start);
		}
		_this->_prof_time_start = time_start;

		_this->__commit();
		io_submit(&_this->_job_update);
	}
}

void
linux_proc_device::io_submit(struct sharaku_job *job)
{
	linux_proc_device *_this = list_entry(job, linux_proc_device, _job_update);

	// __io_submitを呼び出し、procを読み込む。
	// もし、まだ読み込む必要がある場合は自身をスケジュールする。
	int32_t result = _this->__io_submit();
	if (result == EAGAIN || result == EWOULDBLOCK) {
		sharaku_async_message(job, linux_proc_device::io_submit);
	} else if (result != 0) {
		sharaku_db_error("io_submit get_read_flag=0x%08x get_write_flag=0x%08x error=%d",
				 _this->get_read_flag(), _this->get_write_flag(), result, 0, 0, 0);
		io_end(job);
	} else {
		io_end(job);
	}
}

void
linux_proc_device::io_end(struct sharaku_job *job)
{
	linux_proc_device *_this = list_entry(job, linux_proc_device, _job_update);
	_this->__io_end(_this->_type);
	_this->_job = job;

	sharaku_usec_t time_end = sharaku_get_usec();
	switch (_this->_type) {
	case PROC_IOTYPE_READ:
		sharaku_prof_add(&_this->_prof_read_process, _this->_prof_time_start, time_end);
		break;
	case PROC_IOTYPE_WRITE:
		sharaku_prof_add(&_this->_prof_write_process, _this->_prof_time_start, time_end);
		break;
	case PROC_IOTYPE_RW:
		sharaku_prof_add(&_this->_prof_read_process, _this->_prof_time_start, time_end);
		sharaku_prof_add(&_this->_prof_write_process, _this->_prof_time_start, time_end);
		break;
	}
}

NAMESPACE_SHARAKU_END

