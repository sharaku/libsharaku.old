/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_HOOK_GYRO_H_
#define _SHARAKU_DEVICES_HOOK_GYRO_H_

#include <stdint.h>
#include <devices/sensor-api.hpp>
#include <devices/connection_interface.hpp>

NAMESPACE_SHARAKU_BEGIN

class hook_gyro
 : public gyro_operations {
 public:
	hook_gyro() {}
	virtual ~hook_gyro() {}
	// インターフェース接続(複数connect禁止)
	connection_interface<gyro_operations>	out_gyro;

	// インタフェースへのアクセス
	operator gyro_operations*() {
		return (gyro_operations *)this;
	}

 public:
	virtual int32_t	get_angle(void);
	virtual int32_t	get_rate(void);
	virtual int32_t	reset(void);
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_HOOK_GYRO_H_ */
