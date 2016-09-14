/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
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
 , public color_operations
 , public light_operations {
 private:
	ev3dev_color() : ev3dev_lego_sensor(1) {}

 public:
	enum ev3dev_colormode {
		EV3DEV_COLORMODE_UNKNOWN,	// 無効
		EV3DEV_COLORMODE_COLOR,		// 簡易カラーモード
		EV3DEV_COLORMODE_REFLECTED,	// 反射光モード
		EV3DEV_COLORMODE_AMBIENT,	// 環境光モード
		EV3DEV_COLORMODE_CORRECTION,	// 反射光 - 環境光モード
		EV3DEV_COLORMODE_FULLCOLOR,	// フルカラーモード
	};
	ev3dev_color(const char *port, int interval_ms, ev3dev_colormode mode)
	 : ev3dev_lego_sensor(interval_ms) {
		__correction	= 0;
		_reflected	= 0;
		_ambient	= 0;
		_correction	= 0;
		_color		= COLOR_NONE;
		_red		= 0;
		_green		= 0;
		_blue		= 0;
		_mode_sp	= EV3DEV_COLORMODE_UNKNOWN;
		_mode		= EV3DEV_COLORMODE_UNKNOWN;
		set_mode(mode);

		connect(port);
	}

	virtual ~ev3dev_color() {
		disconnect();
	}
	operator color_operations*() {
		return (color_operations *)this;
	}
	operator light_operations*() {
		return (light_operations *)this;
	}

 public:
	light_operations::mode	get_mode(void) {
		switch (_mode) {
		case EV3DEV_COLORMODE_COLOR:
			return light_operations::MODE_COLOR;
		case EV3DEV_COLORMODE_REFLECTED:
			return light_operations::MODE_REFLECTED;
		case EV3DEV_COLORMODE_AMBIENT:
			return light_operations::MODE_AMBIENT;
		case EV3DEV_COLORMODE_CORRECTION:
			return light_operations::MODE_CORRECTION;
		default:
			return light_operations::MODE_UNKNOWN;
		}
	}
	int32_t	set_mode(ev3dev_colormode m) {
		return _mode_sp = m;
	}
	int32_t	get_value(void) {
		switch (_mode) {
		case EV3DEV_COLORMODE_COLOR:
			return _color;
		case EV3DEV_COLORMODE_REFLECTED:
			return _reflected;
		case EV3DEV_COLORMODE_AMBIENT:
			return _ambient;
		case EV3DEV_COLORMODE_CORRECTION:
			return _correction;
		default:
			return -ENOTSUP;
		}
	}
	int32_t	get_red(void) {
		if (_mode != EV3DEV_COLORMODE_FULLCOLOR) {
			return -ENOTSUP;
		} else {
			return _red;
		}
	}
	int32_t	get_green(void) {
		if (_mode != EV3DEV_COLORMODE_FULLCOLOR) {
			return -ENOTSUP;
		} else {
			return _green;
		}
	}
	int32_t	get_blue(void) {
		if (_mode != EV3DEV_COLORMODE_FULLCOLOR) {
			return -ENOTSUP;
		} else {
			return _blue;
		}
	}

 public:
	int32_t connect(const char* port);

 protected:
	void __update(void);
	void __io_end(void);

 private:
	int32_t 		__correction;
	int32_t			_reflected;
	int32_t			_ambient;
	int32_t			_correction;
	int32_t			_color;
	int32_t			_red;
	int32_t			_green;
	int32_t			_blue;
	ev3dev_colormode	_mode_sp;
	ev3dev_colormode	_mode;

	sharaku_prof_t		_prof;
	sharaku_usec_t		_prof_time_start;
	char			_profname[64];
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_COLOR_EV3DEV_COLOR_H_ */
