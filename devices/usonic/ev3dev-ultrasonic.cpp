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
#include <devices/usonic/ev3dev-ultrasonic.hpp>

NAMESPACE_SHARAKU_BEGIN

int32_t
ev3dev_ultrasonic::connect(const char* port)
{
	sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);

	DEVICE_PROC_SET_READ_PROFNAME("ev3dev_ultrasonic<%s", port);

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

void ev3dev_ultrasonic::__commit(void)
{
}

// デバイスから情報を更新する
void ev3dev_ultrasonic::__io_end(PROC_IOTYPE type)
{
	DEVICE_IO_READ
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
	DEVICE_IO_END
}

NAMESPACE_SHARAKU_END

