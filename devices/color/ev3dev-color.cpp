/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#include <stdio.h>
#include <string.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <devices/color/ev3dev-color.hpp>

NAMESPACE_SHARAKU_BEGIN

int32_t ev3dev_color::connect(const char* port)
{
	sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);
	int32_t result = ev3dev_lego_sensor::connect(port);
	if (result != 0) {
		goto err;
	}
	ev3dev_lego_sensor::mode.open();
	ev3dev_lego_sensor::value0.open();
	ev3dev_lego_sensor::value1.open();
	ev3dev_lego_sensor::value2.open();
	ev3dev_lego_sensor::mode = "COL-REFLECT";
	set_write_flag(DEVICE_FILE_MODE);
	_mode = EV3DEV_COLORMODE_REFLECTED;
	_mode_sp = EV3DEV_COLORMODE_REFLECTED;
	sharaku_db_trace("end(0)", 0, 0, 0, 0, 0, 0);
	return 0;

 err:
	sharaku_db_trace("end(-1)", 0, 0, 0, 0, 0, 0);
	return -1;
}

// デバイスから情報を更新する
void ev3dev_color::__update(void)
{
	// モード切替がない場合、かつ現在のモードがEV3DEV_COLORMODE_CORRECTION
	// ではない場合、モード切替をスキップする。
	if (_mode != _mode_sp || _mode == EV3DEV_COLORMODE_CORRECTION) {
		_mode = _mode_sp;
		switch (_mode) {
		case EV3DEV_COLORMODE_COLOR:
			ev3dev_lego_sensor::mode = "COL-COLOR";
			break;
		case EV3DEV_COLORMODE_REFLECTED:
			ev3dev_lego_sensor::mode = "COL-REFLECT";
			break;
		case EV3DEV_COLORMODE_AMBIENT:
			ev3dev_lego_sensor::mode = "COL-AMBIENT";
			break;
		case EV3DEV_COLORMODE_CORRECTION:
			if (__correction) {
				ev3dev_lego_sensor::mode = "COL-REFLECT";
				__correction = 0;
			} else {
				ev3dev_lego_sensor::mode = "COL-AMBIENT";
				__correction = 1;
			}
			break;
		case EV3DEV_COLORMODE_FULLCOLOR:
			ev3dev_lego_sensor::mode = "RGB-RAW";
			break;
		default:
			break;
		}
		set_write_flag(DEVICE_FILE_MODE);
	}

	// モードを書き込む
	switch (_mode) {
	case EV3DEV_COLORMODE_COLOR:
	case EV3DEV_COLORMODE_REFLECTED:
	case EV3DEV_COLORMODE_AMBIENT:
	case EV3DEV_COLORMODE_CORRECTION:
		set_read_flag(DEVICE_FILE_VALUE0);
		break;
	case EV3DEV_COLORMODE_FULLCOLOR:
		set_read_flag(DEVICE_FILE_VALUE0);
		set_read_flag(DEVICE_FILE_VALUE1);
		set_read_flag(DEVICE_FILE_VALUE2);
		break;
	default:
		break;
	}
	sharaku_db_trace("update-flag get_read_flag=0x%08x get_write_flag=0x%08x",
			 get_read_flag(), get_write_flag(), 0, 0, 0, 0);
}


// デバイスから情報を更新する
void ev3dev_color::__io_end(void)
{
	// モードによって次のフェーズ、
	switch (_mode) {
	case EV3DEV_COLORMODE_COLOR:
		_color = ev3dev_lego_sensor::value0;
		sharaku_db_trace("COLOR color=%d", _color, 0, 0, 0, 0, 0);
		break;
	case EV3DEV_COLORMODE_REFLECTED:
		_reflected = ev3dev_lego_sensor::value0;
		sharaku_db_trace("EV3DEV_COLORMODE_REFLECTED reflected=%d",
				 _reflected, 0, 0, 0, 0, 0);
		break;
	case EV3DEV_COLORMODE_AMBIENT:
		_ambient = ev3dev_lego_sensor::value0;
		sharaku_db_trace("AMBIENT ambient=%d",
				 _ambient, 0, 0, 0, 0, 0);
		break;
	case EV3DEV_COLORMODE_CORRECTION:
		if (__correction) {
			_reflected = ev3dev_lego_sensor::value0;
		} else {
			_ambient = ev3dev_lego_sensor::value0;
		}
		sharaku_db_trace("CORRECTION reflected=%d ambient=%d",
				 _reflected, _ambient, 0, 0, 0, 0);
		_correction = _reflected - _ambient;
		break;
	case EV3DEV_COLORMODE_FULLCOLOR:
		_red	= ev3dev_lego_sensor::value0;
		_green	= ev3dev_lego_sensor::value1;
		_blue	= ev3dev_lego_sensor::value2;
		sharaku_db_trace("FULLCOLOR red=%d green=%d blue=%d",
				 _red, _green, _blue, 0, 0, 0);
		break;
	default:
		sharaku_db_trace("unknown mode.", 0, 0, 0, 0, 0, 0);
		break;
	}
}

NAMESPACE_SHARAKU_END

