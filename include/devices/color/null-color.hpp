/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_NULL_COLOR_H_
#define _SHARAKU_DEVICES_NULL_COLOR_H_

#include <stdint.h>
#include <devices/sensor-api.hpp>

NAMESPACE_SHARAKU_BEGIN

class null_color
 : public color_operations
 , public light_operations {
 private:
	null_color() {}
	virtual ~null_color() {}
	operator color_operations*() {
		return (color_operations *)this;
	}
	operator light_operations*() {
		return (light_operations *)this;
	}

 public:
	virtual int32_t	set_mode(ev3dev_colormode m) {
		return 0;
	}
	virtual int32_t	get_value(void) {
		return 0;
	}
	virtual int32_t	get_red(void) {
		return 0;
	}
	int32_t	get_green(void) {
		return 0;
	}
	virtual int32_t	get_blue(void) {
		return 0;
	}
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_EV3DEV_COLOR_H_ */
