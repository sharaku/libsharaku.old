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
#include <devices/power/ev3dev-power-supply.hpp>

NAMESPACE_SHARAKU_BEGIN

int32_t ev3dev_powersupply::connect(void)
{
	sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);

	DEVICE_PROC_SET_READ_PROFNAME("ev3dev_powersupply");

	int32_t result = ev3dev_lego_powersupply::connect();
	if (result != 0) {
		goto err;
	}
	ev3dev_lego_powersupply::current_now.open();
	ev3dev_lego_powersupply::voltage_now.open();
	set_read_flag(DEVICE_FILE_CHARGE_FULL);
	set_read_flag(DEVICE_FILE_CHARGE_NOW);
	set_read_flag(DEVICE_FILE_CURRENT_NOW);
	set_read_flag(DEVICE_FILE_VOLTAGE_NOW);
	set_read_flag(DEVICE_FILE_VOLTAGE_MAX);
	set_read_flag(DEVICE_FILE_VOLTAGE_MIN);
	set_read_flag(DEVICE_FILE_STATUS);
	set_read_flag(DEVICE_FILE_TYPE);
	sharaku_db_trace("end(0)", 0, 0, 0, 0, 0, 0);
	return 0;

 err:
	sharaku_db_trace("end(-1)", 0, 0, 0, 0, 0, 0);
	return -1;
}

// デバイスから情報を更新する
void ev3dev_powersupply::__update(void)
{
	set_read_flag(DEVICE_FILE_CURRENT_NOW);
	set_read_flag(DEVICE_FILE_VOLTAGE_NOW);
	sharaku_db_trace("update-flag get_read_flag=0x%08x get_write_flag=0x%08x",
			 get_read_flag(), get_write_flag(), 0, 0, 0, 0);
}

void ev3dev_powersupply::__commit(void)
{
}

// デバイスから情報を更新する
void ev3dev_powersupply::__io_end(PROC_IOTYPE type)
{
	DEVICE_IO_READ
		_current = ev3dev_lego_powersupply::current_now / 1000;
		_voltage = ev3dev_lego_powersupply::voltage_now / 1000;
		_voltage_max = ev3dev_lego_powersupply::voltage_max_design / 1000;
		_voltage_min = ev3dev_lego_powersupply::voltage_min_design / 1000;
		sharaku_db_trace("current=%u _voltage=%u _voltage_max=%u _voltage_min=%u",
				 _current, _voltage, _voltage_max, _voltage_min, 0, 0);
	DEVICE_IO_END
}

NAMESPACE_SHARAKU_END

