/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_NULL_ULTRASONIC_H_
#define _SHARAKU_DEVICES_NULL_ULTRASONIC_H_

#include <stdint.h>
#include <devices/api.hpp>

NAMESPACE_SHARAKU_BEGIN

class null_ultrasonic
 : public usonic_operations {
 public:
	null_ultrasonic() {}
	virtual ~null_ultrasonic(){}
	operator usonic_operations*() {
		return (usonic_operations*)this;
	}

 public:
	virtual int32_t set_uint(uint u) {
		return 0;
	}
	virtual int32_t	get_dist(void) {
		return 0;
	}
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_NULL_ULTRASONIC_H_ */
