/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_UV_DEVICE_EV3DEV_TOUCH_H_
#define SHARAKU_UV_DEVICE_EV3DEV_TOUCH_H_

#include <stdint.h>
#include <sharaku/log.h>
#include <devices/api.hpp>
#include <devices/proc/ev3dev-proc-device.hpp>

NAMESPACE_SHARAKU_BEGIN

class ev3dev_touch
 : protected ev3dev_lego_sensor
 , public touch_operations {
 private:
	ev3dev_touch() : ev3dev_lego_sensor(1) {}

 public:
	ev3dev_touch(const char *port, int interval_ms)
	 : ev3dev_lego_sensor(interval_ms) {
		sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);
		_press		= 0;
		_push		= 0;
		_release	= 0;

		ev3dev_touch::connect(port);
		sharaku_db_trace("end", 0, 0, 0, 0, 0, 0);
	}
	virtual ~ev3dev_touch() {
		disconnect();
	}
	operator touch_operations*() {
		return (touch_operations *)this;
	}

 public:
	int32_t is_press(void) {
		return _press;
	}
	int32_t is_push(void) {
		return _push;
	}
	int32_t is_release(void) {
		return _release;
	}

 public:
	int32_t connect(const char* port);

 protected:
	void __update(void);
	void __io_end(void);

 private:
	int32_t		_press;
	int32_t		_push;
	int32_t		_release;
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_UV_DEVICE_EV3DEV_LARGE_MOTOR_H_