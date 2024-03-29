﻿/* --
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
#include <devices/color/ev3dev-color.hpp>

NAMESPACE_SHARAKU_BEGIN

int32_t ev3dev_color::connect(const char* port)
{
	sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);

	DEVICE_PROC_SET_READ_PROFNAME("ev3dev_color<%s", port);

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
	_mode = color_operations::MODE_REFLECTED;
	_mode_sp = color_operations::MODE_REFLECTED;
	sharaku_db_trace("end(0)", 0, 0, 0, 0, 0, 0);
	return 0;

 err:
	sharaku_db_trace("end(-1)", 0, 0, 0, 0, 0, 0);
	return -1;
}

// デバイスから情報を更新する
void ev3dev_color::__update(void)
{
	// モード切替がない場合、かつ現在のモードがcolor_operations::MODE_CORRECTION
	// ではない場合、モード切替をスキップする。
	if (_mode != _mode_sp || _mode == color_operations::MODE_CORRECTION) {
		_mode = _mode_sp;
		switch (_mode) {
		case color_operations::MODE_REFLECTED:
			ev3dev_lego_sensor::mode = "COL-REFLECT";
			break;
		case color_operations::MODE_AMBIENT:
			ev3dev_lego_sensor::mode = "COL-AMBIENT";
			break;
		case color_operations::MODE_CORRECTION:
			if (__correction) {
				ev3dev_lego_sensor::mode = "COL-REFLECT";
				__correction = 0;
			} else {
				ev3dev_lego_sensor::mode = "COL-AMBIENT";
				__correction = 1;
			}
			break;
		case color_operations::MODE_FULLCOLOR:
			ev3dev_lego_sensor::mode = "RGB-RAW";
			break;
		default:
			break;
		}
		set_write_flag(DEVICE_FILE_MODE);
	}

	// モードを書き込む
	switch (_mode) {
	case color_operations::MODE_REFLECTED:
	case color_operations::MODE_AMBIENT:
	case color_operations::MODE_CORRECTION:
		set_read_flag(DEVICE_FILE_VALUE0);
		break;
	case color_operations::MODE_FULLCOLOR:
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

void ev3dev_color::__commit(void)
{
}

// デバイスから情報を更新する
void ev3dev_color::__io_end(PROC_IOTYPE type)
{
	DEVICE_IO_READ
		switch (_mode) {
		case color_operations::MODE_REFLECTED:
			_reflected = ev3dev_lego_sensor::value0;
			sharaku_db_trace("color_operations::MODE_REFLECTED reflected=%d",
					 _reflected, 0, 0, 0, 0, 0);
			break;
		case color_operations::MODE_AMBIENT:
			_ambient = ev3dev_lego_sensor::value0;
			sharaku_db_trace("AMBIENT ambient=%d",
					 _ambient, 0, 0, 0, 0, 0);
			break;
		case color_operations::MODE_CORRECTION:
			if (__correction) {
				_reflected = ev3dev_lego_sensor::value0;
			} else {
				_ambient = ev3dev_lego_sensor::value0;
			}
			sharaku_db_trace("CORRECTION reflected=%d ambient=%d",
					 _reflected, _ambient, 0, 0, 0, 0);
			_correction = _reflected - _ambient;
			break;
		case color_operations::MODE_FULLCOLOR:
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
	DEVICE_IO_END
}

NAMESPACE_SHARAKU_END

