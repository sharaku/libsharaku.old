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

#ifndef _SHARAKU_DEVICES_COLOR_EV3DEV_COLOR_H_
#define _SHARAKU_DEVICES_COLOR_EV3DEV_COLOR_H_

#include <stdint.h>
#include <devices/sensor-api.hpp>
#include <devices/proc/ev3dev-proc-device.hpp>

NAMESPACE_SHARAKU_BEGIN

class ev3dev_color
 : protected ev3dev_lego_sensor
 , public color_operations {
 private:
	ev3dev_color() : ev3dev_lego_sensor(1) {}

 public:
	ev3dev_color(const char *port, int interval_ms, color_operations::mode mode)
	 : ev3dev_lego_sensor(interval_ms) {
		__correction	= 0;
		_reflected	= 0;
		_ambient	= 0;
		_correction	= 0;
		_red		= 0;
		_green		= 0;
		_blue		= 0;
		_mode_sp	= color_operations::MODE_UNKNOWN;
		_mode		= color_operations::MODE_UNKNOWN;
		set_mode(mode);

		connect(port);
	}

	virtual ~ev3dev_color() {
		disconnect();
	}
	operator color_operations*() {
		return (color_operations *)this;
	}

 public:
	color_operations::mode	get_mode(void) {
		return _mode;
	}
	int32_t	set_mode(color_operations::mode m) {
		return _mode_sp = m;
	}
	int32_t	get_value(void) {
		switch (_mode) {
		case color_operations::MODE_REFLECTED:
			return _reflected;
		case color_operations::MODE_AMBIENT:
			return _ambient;
		case color_operations::MODE_CORRECTION:
			return _correction;
		default:
			return -ENOTSUP;
		}
	}
	int32_t	get_red(void) {
		if (_mode != color_operations::MODE_FULLCOLOR) {
			return -ENOTSUP;
		} else {
			return _red;
		}
	}
	int32_t	get_green(void) {
		if (_mode != color_operations::MODE_FULLCOLOR) {
			return -ENOTSUP;
		} else {
			return _green;
		}
	}
	int32_t	get_blue(void) {
		if (_mode != color_operations::MODE_FULLCOLOR) {
			return -ENOTSUP;
		} else {
			return _blue;
		}
	}

 public:
	int32_t connect(const char* port);

 protected:
	void __update(void);
	void __commit(void);
	void __io_end(PROC_IOTYPE type);

 private:
	int32_t 		__correction;
	int32_t			_reflected;
	int32_t			_ambient;
	int32_t			_correction;
	int32_t			_red;
	int32_t			_green;
	int32_t			_blue;
	color_operations::mode	_mode_sp;
	color_operations::mode	_mode;
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_COLOR_EV3DEV_COLOR_H_ */
