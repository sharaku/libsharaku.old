/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#include <stdio.h>
#include <string.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <devices/api.hpp>
#include <devices/proc/linux-proc-device.hpp>

NAMESPACE_SHARAKU_BEGIN

// 定期的にprocを読み込んで更新する
void
linux_proc_device::update_device(struct sharaku_job *job)
{
	linux_proc_device *_this = list_entry(job, linux_proc_device, _job_interval);
	// 次をスケジュール
	sharaku_timer_message(job,
			      _this->_interval_ms,
			      linux_proc_device::update_device);

	// updateの必要があればupdateする
	if (_this->_job) {
		_this->_job = NULL;
		_this->__update();
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
	_this->__io_end();
	_this->_job = job;
}

NAMESPACE_SHARAKU_END

