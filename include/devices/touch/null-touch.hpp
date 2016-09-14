/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_NULL_TOUCH_H_
#define _SHARAKU_DEVICES_NULL_TOUCH_H_

#include <stdint.h>
#include <devices/sensor-api.hpp>

NAMESPACE_SHARAKU_BEGIN

class null_touch
 : public touch_operations {
 public:
	null_touch() {}
	virtual ~null_touch() {}
	operator touch_operations*() {
		return (touch_operations *)this;
	}

 public:
	virtual int32_t is_press(void) {
		return 0;
	}
	virtual int32_t is_push(void) {
		return 0;
	}
	virtual int32_t is_release(void) {
		return 0;
	}
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_NULL_TOUCH_H_ */
