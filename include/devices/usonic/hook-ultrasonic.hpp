/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_HOOK_ULTRASONIC_H_
#define _SHARAKU_DEVICES_HOOK_ULTRASONIC_H_

#include <stdint.h>
#include <devices/sensor-api.hpp>
#include <devices/connection_interface.hpp>

NAMESPACE_SHARAKU_BEGIN

class hook_ultrasonic
 : public usonic_operations {
 public:
	hook_ultrasonic() {}
	virtual ~hook_ultrasonic(){}
	// インターフェース接続(複数connect禁止)
	connection_interface<usonic_operations> out_usonic;

	// インタフェースへのアクセス
	operator usonic_operations*() {
		return (usonic_operations*)this;
	}

 public:
	virtual int32_t set_uint(usonic_operations::uint u);
	virtual int32_t	get_dist(void);
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_HOOK_ULTRASONIC_H_ */
