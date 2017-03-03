/* --
 *
 * MIT License
 * 
 * Copyright (c) 2016 Abe Takafumi
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

#ifndef __SHARAKU_DEVICES_EV3RT_COLOR_H_
#define __SHARAKU_DEVICES_EV3RT_COLOR_H_

#include <stdint.h>
#include <sharaku/types.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <ev3api.h>
#include <devices/sensor-api.hpp>
#include <devices/ev3rt-device-def.h>

NAMESPACE_SHARAKU_BEGIN

class ev3rt_color
 : public color_operations {
 public:
	ev3rt_color(sensor_port_t port, color_operations::mode mode) {
		_port		= port;
		__correction	= 0;
		_reflected	= 0;
		_ambient	= 0;
		_correction	= 0;
		_red		= 0;
		_green		= 0;
		_blue		= 0;
		_mode		= mode;
		ev3_sensor_config(_port, COLOR_SENSOR);
	}
	virtual ~ev3rt_color() {}

	// インタフェースへのアクセス
	operator color_operations*() {
		return (color_operations *)this;
	}

 public:
	virtual int32_t	set_mode(color_operations::mode m) {
		switch (m) {
		case MODE_REFLECTED:
			ev3_color_sensor_get_reflect(_port);
			break;
		case MODE_AMBIENT:
			ev3_color_sensor_get_ambient(_port);
			break;
		case MODE_CORRECTION:
			_reflected = ev3_color_sensor_get_reflect(_port);
			break;
		case MODE_FULLCOLOR:
			break;
		default:
			break;
		}
		_mode = m;
		return 0;
	}
	virtual color_operations::mode	get_mode(void) {
		return _mode;
	}
	virtual int32_t	get_value(void) {
		__update();
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
	virtual int32_t	get_red(void) {
		if (_mode != color_operations::MODE_FULLCOLOR) {
			return -ENOTSUP;
		} else {
			__update();
			return _red;
		}
	}
	virtual int32_t get_green(void) {
		if (_mode != color_operations::MODE_FULLCOLOR) {
			return -ENOTSUP;
		} else {
			__update();
			return _green;
		}
	}
	virtual int32_t	get_blue(void) {
		if (_mode != color_operations::MODE_FULLCOLOR) {
			return -ENOTSUP;
		} else {
			__update();
			return _blue;
		}
	}

 private:
	void __update(void) {
//		rgb_raw_t val;

		switch (_mode) {
		case color_operations::MODE_REFLECTED:
			_reflected = ev3_color_sensor_get_reflect(_port);
			break;
		case color_operations::MODE_AMBIENT:
			_ambient = ev3_color_sensor_get_ambient(_port);
			break;
		case color_operations::MODE_CORRECTION:
			if (__correction) {
				_reflected = ev3_color_sensor_get_reflect(_port);
				__correction = 0;
			} else {
				_ambient = ev3_color_sensor_get_ambient(_port);
				__correction = 1;
			}
			_correction = _reflected - _ambient;
			break;
		case color_operations::MODE_FULLCOLOR:
//			ev3_color_sensor_get_rgb_raw(_port, &val);
//			_red	= val.r;
//			_green	= val.g;
//			_blue	= val.b;
			break;
		default:
			break;
		}
	}

 private:
	sensor_port_t		_port;
	int32_t 		__correction;
	int32_t			_reflected;
	int32_t			_ambient;
	int32_t			_correction;
	int32_t			_red;
	int32_t			_green;
	int32_t			_blue;
	color_operations::mode	_mode;
};

NAMESPACE_SHARAKU_END

#endif /* __SHARAKU_DEVICES_EV3RT_COLOR_H_ */
