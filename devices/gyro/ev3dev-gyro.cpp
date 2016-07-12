/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#include <stdio.h>
#include <string.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <devices/gyro/ev3dev-gyro.hpp>

// Gyro offset control
// The HiTechnic gyro sensor will drift with time.  This constant is
// used in a simple long term averaging to adjust for this drift.
// Every time through the loop, the current gyro sensor value is
// averaged into the gyro offset weighted according to this constant.
const float EMAOFFSET = 0.0005f;

NAMESPACE_SHARAKU_BEGIN

int32_t ev3dev_gyro::connect(const char* port)
{
	sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);

	snprintf(_profname, 64, "ev3dev_gyro<%s", port);
	char *work = strstr(_profname, "\n");
	*work = '>';
	sharaku_prof_init(&_prof, _profname);
	sharaku_prof_regist(&_prof);

	int32_t result = ev3dev_lego_sensor::connect(port);
	if (result != 0) {
		goto err;
	}
	ev3dev_lego_sensor::value0.open();
	ev3dev_lego_sensor::value1.open();
	ev3dev_lego_sensor::mode = "GYRO-G&A";
	set_write_flag(DEVICE_FILE_MODE);
	_start_time = sharaku_get_usec();
	sharaku_db_trace("end(0)", 0, 0, 0, 0, 0, 0);
	return 0;

 err:
	sharaku_db_trace("end(-1)", 0, 0, 0, 0, 0, 0);
	return -1;
}

// デバイスから情報を更新する
void ev3dev_gyro::__update(void)
{
	_prof_time_start = sharaku_get_usec();

	set_read_flag(DEVICE_FILE_VALUE0);
	set_read_flag(DEVICE_FILE_VALUE1);
	sharaku_db_trace("update-flag get_read_flag=0x%08x get_write_flag=0x%08x",
			 get_read_flag(), get_write_flag(), 0, 0, 0, 0);
}


// デバイスから情報を更新する
void ev3dev_gyro::__io_end(void)
{
	int32_t value0_angle = ev3dev_lego_sensor::value0 * _sign;
	int32_t value1_rate = ev3dev_lego_sensor::value1 * _sign;

	// 平均インターバルを計算する
	_count ++;
	sharaku_usec_t time = sharaku_get_usec();
	float interval	= (float)(time - _start_time) / (float)_count /1000000.0f;

	if(_is_reset) {
		// キャリブレーションを実行
		if (_reset_counter < 100) {
			_total += value1_rate;
			_reset_counter ++;
		} else {
			float average	= _total / 100.0f;
			_offset		= average - 0.1f;
			_base_angle	= value0_angle;
			_is_reset	= 0;
		}
		sharaku_db_trace("reset _reset_counter=%d",
				 _reset_counter, 0, 0, 0, 0, 0);
	} else {
		// デバイスから入力を行う. ドライバから情報をもらった方が正確.
		_angle				= value0_angle;
		register int32_t gyro_raw	= value1_rate;

		// 情報の組み立てを行う
		_offset	= EMAOFFSET * gyro_raw + (1.0f - EMAOFFSET) * _offset;
		_rate	= ((float)gyro_raw - _offset);

		sharaku_db_trace("angle(value0)=%d rate=%d base_angle=%d",
				 _angle, _rate, _base_angle, 0, 0, 0);
	}

	sharaku_usec_t time_end = sharaku_get_usec();
	sharaku_prof_add(&_prof, _prof_time_start, time_end);
}

NAMESPACE_SHARAKU_END

