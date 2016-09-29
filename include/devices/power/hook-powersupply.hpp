/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_HOOK_POWERSUPPLY_H_
#define _SHARAKU_DEVICES_HOOK_POWERSUPPLY_H_

#include <stdint.h>
#include <devices/sensor-api.hpp>
#include <devices/connection_interface.hpp>

NAMESPACE_SHARAKU_BEGIN

class hook_powersupply
 : public powersupply_operations {
 public:
	hook_powersupply() {}
	virtual ~hook_powersupply(){}
	// インターフェース接続(複数connect禁止)
	connection_interface<powersupply_operations> out_power;

	// インタフェースへのアクセス
	operator powersupply_operations*() {
		return (powersupply_operations*)this;
	}

 public:
	virtual int32_t	get_voltage(void);
	virtual int32_t	get_voltage_max(void);
	virtual int32_t	get_voltage_min(void);
	virtual int32_t	get_current(void);
	virtual int32_t	get_battery_type(void);
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_HOOK_POWERSUPPLY_H_ */
