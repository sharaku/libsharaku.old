/* --
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
	linux_proc_device *_this =
			list_entry(job, linux_proc_device, _job_interval);

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

		sharaku_usec_t time_start = sharaku_get_usec();
		if (_this->_prof_time_read_start) {
			sharaku_prof_add(&_this->_prof_read_interval, _this->_prof_time_read_start, time_start);
		}
		if (_this->_prof_time_write_start) {
			sharaku_prof_add(&_this->_prof_write_interval, _this->_prof_time_write_start, time_start);
		}
		_this->_prof_time_read_start = time_start;
		_this->_prof_time_write_start = time_start;

		_this->__update();
		_this->__commit();

		// flagが0以外であれば、スケジュールする。
		// 0であればスケジュールしない。
		if (_this->_write_flags | _this->_read_flags) {
			_this->_job = NULL;
			io_submit(&_this->_job_update);
		} else {
			_this->_job_i = &_this->_job_interval;
		}
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

		sharaku_usec_t time_start = sharaku_get_usec();
		if (_this->_prof_time_read_start) {
			sharaku_prof_add(&_this->_prof_read_interval, _this->_prof_time_read_start, time_start);
		}
		_this->_prof_time_read_start = time_start;

		_this->__update();
		// flagが0以外であれば、スケジュールする。
		// 0であればスケジュールしない。
		if (_this->_write_flags | _this->_read_flags) {
			_this->_job = NULL;
			io_submit(&_this->_job_update);
		} else {
			_this->_job_i = &_this->_job_interval;
		}
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

		sharaku_usec_t time_start = sharaku_get_usec();
		if (_this->_prof_time_write_start) {
			sharaku_prof_add(&_this->_prof_write_interval, _this->_prof_time_write_start, time_start);
		}
		_this->_prof_time_write_start = time_start;

		_this->__commit();
		if (_this->_write_flags | _this->_read_flags) {
			_this->_job = NULL;
			io_submit(&_this->_job_update);
		} else {
			_this->_job_i = &_this->_job_interval;
		}
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
		switch (_this->_type) {
		case PROC_IOTYPE_READ:
			sharaku_prof_add_notime(&_this->_prof_read_count);
			break;
		case PROC_IOTYPE_WRITE:
			sharaku_prof_add_notime(&_this->_prof_write_count);
			break;
		case PROC_IOTYPE_RW:
			sharaku_prof_add_notime(&_this->_prof_read_count);
			sharaku_prof_add_notime(&_this->_prof_write_count);
			break;
		}
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
		sharaku_prof_add(&_this->_prof_read_process, _this->_prof_time_read_start, time_end);
		break;
	case PROC_IOTYPE_WRITE:
		sharaku_prof_add(&_this->_prof_write_process, _this->_prof_time_write_start, time_end);
		break;
	case PROC_IOTYPE_RW:
		sharaku_prof_add(&_this->_prof_read_process, _this->_prof_time_read_start, time_end);
		sharaku_prof_add(&_this->_prof_write_process, _this->_prof_time_write_start, time_end);
		break;
	}
}

NAMESPACE_SHARAKU_END

