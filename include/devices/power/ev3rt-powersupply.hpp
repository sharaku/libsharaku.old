/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_EV3RT_POWERSUPPLY_H_
#define _SHARAKU_DEVICES_EV3RT_POWERSUPPLY_H_

#include <stdint.h>
#include <sharaku/types.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <ev3api.h>
#include <devices/sensor-api.hpp>

NAMESPACE_SHARAKU_BEGIN

class ev3rt_powersupply
 : public powersupply_operations {
 public:
	ev3rt_powersupply() {}
	virtual ~ev3rt_powersupply(){}

	operator powersupply_operations*() {
		return (powersupply_operations*)this;
	}

 public:
	virtual int32_t	get_voltage(void) {
		return ev3_battery_voltage_mV();
	}
	virtual int32_t	get_voltage_max(void) {
		return 10000;
	}
	virtual int32_t	get_voltage_min(void) {
		return 6000;
	}
	virtual int32_t	get_current(void) {
		return ev3_battery_current_mA();
	}
	virtual int32_t	get_battery_type(void) {
		return ENOTSUP;
	}
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_HOOK_POWERSUPPLY_H_ */
