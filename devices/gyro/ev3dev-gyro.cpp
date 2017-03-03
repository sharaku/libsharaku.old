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

	DEVICE_PROC_SET_READ_PROFNAME("ev3dev_gyro<%s", port);

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
	set_read_flag(DEVICE_FILE_VALUE0);
	set_read_flag(DEVICE_FILE_VALUE1);
	sharaku_db_trace("update-flag get_read_flag=0x%08x get_write_flag=0x%08x",
			 get_read_flag(), get_write_flag(), 0, 0, 0, 0);
}

void ev3dev_gyro::__commit(void)
{
}

// デバイスから情報を更新する
void ev3dev_gyro::__io_end(PROC_IOTYPE type)
{
	DEVICE_IO_READ
		int32_t value0_angle = ev3dev_lego_sensor::value0 * _sign;
		int32_t value1_rate = ev3dev_lego_sensor::value1 * _sign;

		// 平均インターバルを計算する
		_count ++;

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
	DEVICE_IO_END
}

NAMESPACE_SHARAKU_END

