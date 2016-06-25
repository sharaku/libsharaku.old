/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_NULL_GYRO_H_
#define _SHARAKU_DEVICES_NULL_GYRO_H_

#include <stdint.h>
#include <devices/api.hpp>

NAMESPACE_SHARAKU_BEGIN

class null_gyro
 : public gyro_operations {
 public:
	null_gyro() {}
	virtual ~null_gyro() {}
	// インタフェースへのアクセス
	operator gyro_operations*() {
		return (gyro_operations *)this;
	}

 public:
	virtual int32_t set_sign(int32_t s) {
		return 0;
	}
	// gyro_operations API
	virtual int32_t	get_angle(void) {
		return 0;
	}
	virtual int32_t	get_rate(void) {
		return 0;
	}
	virtual int32_t	reset(void) {
		return 0;
	}
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_NULL_GYRO_H_ */
