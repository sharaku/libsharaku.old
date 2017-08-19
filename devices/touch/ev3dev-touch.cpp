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
#include <devices/touch/ev3dev-touch.hpp>

NAMESPACE_SHARAKU_BEGIN

int32_t ev3dev_touch::connect(const char* port)
{
	sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);

	DEVICE_PROC_SET_READ_PROFNAME("ev3dev_touch<%s", port);

	int32_t result = ev3dev_lego_sensor::connect(port);
	if (result != 0) {
		goto err;
	}
	ev3dev_lego_sensor::value0.open();
	ev3dev_lego_sensor::mode = "TOUCH";
	set_write_flag(DEVICE_FILE_MODE);
	sharaku_db_trace("end(0)", 0, 0, 0, 0, 0, 0);
	return 0;

 err:
	sharaku_db_trace("end(-1)", 0, 0, 0, 0, 0, 0);
	return -1;
}

// デバイスから情報を更新する
void ev3dev_touch::__update(void)
{
	set_read_flag(DEVICE_FILE_VALUE0);
	sharaku_db_trace("update-flag get_read_flag=0x%08x get_write_flag=0x%08x",
			 get_read_flag(), get_write_flag(), 0, 0, 0, 0);
}

void ev3dev_touch::__commit(void)
{
}

// デバイスから情報を更新する
void ev3dev_touch::__io_end(PROC_IOTYPE type)
{
	DEVICE_IO_READ
		int32_t touch = ev3dev_lego_sensor::value0;

		if (_press != touch) {
			// 変化があった。
			if (touch != 0) {
				_push		= 1;
			}
			if (touch == 0) {
				_release	= 1;
			}
		} else {
			// 変化がなかった場合、クリアはしない。
		}

		// 情報の組み立てを行う
		_press	= touch;
		sharaku_db_trace("_press=%u _push=%u _release=%u",
				 _press, _push, _release, 0, 0, 0);
	DEVICE_IO_END
}

NAMESPACE_SHARAKU_END

