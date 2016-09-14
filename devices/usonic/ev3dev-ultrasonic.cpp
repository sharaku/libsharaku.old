/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#include <stdio.h>
#include <string.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <devices/usonic/ev3dev-ultrasonic.hpp>

NAMESPACE_SHARAKU_BEGIN

int32_t
ev3dev_ultrasonic::connect(const char* port)
{
	sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);

	snprintf(_profname, 64, "ev3dev_ultrasonic<%s", port);
	char *work = strstr(_profname, "\n");
	*work = '>';
	sharaku_prof_init(&_prof, _profname);
	sharaku_prof_regist(&_prof);

	int32_t result = ev3dev_lego_sensor::connect(port);
	if (result != 0) {
		goto err;
	}
	ev3dev_lego_sensor::value0.open();
	ev3dev_lego_sensor::mode = "US-DIST-CM";
	set_write_flag(DEVICE_FILE_MODE);
	_uint_sp = USONIC_MM;
	_uint = USONIC_MM;
	sharaku_db_trace("end(0)", 0, 0, 0, 0, 0, 0);
	return 0;

 err:
	sharaku_db_trace("end(-1)", 0, 0, 0, 0, 0, 0);
	return -1;
}

// デバイスから情報を更新する
void ev3dev_ultrasonic::__update(void)
{
	_prof_time_start = sharaku_get_usec();

	if (_uint_sp != _uint) {
		switch (_uint_sp) {
		case USONIC_MM:
			ev3dev_lego_sensor::mode = "US-DIST-CM";
			break;
		case USONIC_IN:
			ev3dev_lego_sensor::mode = "US-DIST-IN";
			break;
		}
		set_write_flag(DEVICE_FILE_MODE);
		_uint = _uint_sp;
	}
	set_read_flag(DEVICE_FILE_VALUE0);
	sharaku_db_trace("update-flag get_read_flag=0x%08x get_write_flag=0x%08x",
			 get_read_flag(), get_write_flag(), 0, 0, 0, 0);
}


// デバイスから情報を更新する
void ev3dev_ultrasonic::__io_end(void)
{
	switch (_uint) {
	case USONIC_MM:
		_dist_mm = ev3dev_lego_sensor::value0;
		sharaku_db_trace("usonic mm=%u.", _dist_mm, 0, 0, 0, 0, 0);
		break;
	case USONIC_IN:
		_dist_in = ev3dev_lego_sensor::value0;
		sharaku_db_trace("usonic in=%u.", _dist_in, 0, 0, 0, 0, 0);
		break;
	}

	sharaku_usec_t time_end = sharaku_get_usec();
	sharaku_prof_add(&_prof, _prof_time_start, time_end);
}

NAMESPACE_SHARAKU_END

