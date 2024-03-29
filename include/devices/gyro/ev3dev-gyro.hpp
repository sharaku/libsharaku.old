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
 * SOFTWARE. *
 *
 */

#ifndef SHARAKU_DEVICES_EV3DEV_GYRO_H_
#define SHARAKU_DEVICES_EV3DEV_GYRO_H_

#include <stdint.h>
#include <devices/sensor-api.hpp>
#include <devices/proc/ev3dev-proc-device.hpp>

NAMESPACE_SHARAKU_BEGIN

class ev3dev_gyro
 : protected ev3dev_lego_sensor
 , public gyro_operations {
 private:
	ev3dev_gyro() : ev3dev_lego_sensor(1) {}

 public:
	ev3dev_gyro(const char *port, int interval_ms)
	 : ev3dev_lego_sensor(interval_ms) {
		_base_angle	= 0;
		_0_offset	= 0;
		_sign		= 1;
		_sign_sp	= 1;
		_offset		= 0;
		_reset_sp	= 0;
		_reset_counter	= 0;
		_reserve	= 0;
		_total		= 0;
		_count		= 0;
		_rate		= 0;
		_angle		= 0;

		connect(port);
	}
	virtual ~ev3dev_gyro() {
		disconnect();
	}
	// インタフェースへのアクセス
	operator gyro_operations*() {
		return (gyro_operations *)this;
	}
	operator device_update_operations*() {
		return (device_update_operations *)this;
	}

 public:
	int32_t set_sign(int32_t s) {
		if (s != 1 && s != -1) {
			return -EINVAL;
		}
		return _sign_sp = s;
	}
	// gyro_operations API
	virtual int32_t	set_offset(int32_t offset) {
		_0_offset = offset;
		return _0_offset;
	}
	virtual int32_t	get_angle(void) {
		return _angle - _base_angle - _0_offset;
	}
	virtual int32_t	get_rate(void) {
		return _rate;
	}
	virtual int32_t	reset(void) {
		_reset_sp = 1;
		_is_reset = 1;
		return _reset_sp;
	}

 public:
	int32_t connect(const char* port);

 protected:
	void __update(void);
	void __commit(void);
	void __io_end(PROC_IOTYPE type);

 private:
	sharaku_usec_t	_start_time;
	int32_t		_0_offset;
	int32_t		_base_angle;
	int32_t		_sign;
	int32_t		_sign_sp;
	float		_offset;
	float		_reset_sp;
	uint8_t		_is_reset;
	uint8_t		_reset_counter;
	uint16_t	_reserve;
	int32_t		_total;
	int32_t		_count;
	float		_rate;
	float		_angle;
};

NAMESPACE_SHARAKU_END

#endif /* SHARAKU_DEVICES_EV3DEV_GYRO_H_ */
