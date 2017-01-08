/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_EV3RT_TOUCH_H_
#define _SHARAKU_DEVICES_EV3RT_TOUCH_H_

#include <stdint.h>
#include <sharaku/types.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <ev3api.h>
#include <devices/sensor-api.hpp>
#include <devices/ev3rt-device-def.h>

NAMESPACE_SHARAKU_BEGIN

class ev3rt_touch
 : public touch_operations {
 private:
	ev3rt_touch() {}

 public:
	ev3rt_touch(sensor_port_t port) {
		_port = port;
		_press = 0;
		_push = 0;
		_release = 0;
		ev3_sensor_config(_port, TOUCH_SENSOR);
	}
	virtual ~ev3rt_touch() {
	}
	operator touch_operations*() {
		return (touch_operations *)this;
	}

 public:
	int32_t is_press(void) {
		__update();
		return _press;
	}
	int32_t is_push(void) {
		__update();
		// 一度回収したら、状態をクリアする。
		// これを行わないと、APIを使う側で1を取り出せない
		int32_t ret = _push;
		_push = 0;
		return ret;
	}
	int32_t is_release(void) {
		__update();
		// 一度回収したら、状態をクリアする。
		// これを行わないと、APIを使う側で1を取り出せない
		int32_t ret = _release;
		_release = 0;
		return ret;
	}

 private:
	void __update(void) {
		int32_t press = ev3_touch_sensor_is_pressed(_port) ? 1 : 0;

		if (_press != press) {
			// 変化があった。
			if (press != 0) {
				_push		= 1;
			}
			if (press == 0) {
				_release	= 1;
			}
		} else {
			// 変化がなかった場合、クリアはしない。
		}

		// 情報の組み立てを行う
		_press = (press) ? 1 : 0;
	}

 private:
	sensor_port_t	_port;
	int32_t		_press;
	int32_t		_push;
	int32_t		_release;
};

NAMESPACE_SHARAKU_END

#endif // _SHARAKU_DEVICES_EV3RT_TOUCH_H_
