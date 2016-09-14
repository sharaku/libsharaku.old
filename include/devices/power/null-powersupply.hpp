/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_NULL_POWERSUPPLY_H_
#define _SHARAKU_DEVICES_NULL_POWERSUPPLY_H_

#include <stdint.h>
#include <devices/sensor-api.hpp>

NAMESPACE_SHARAKU_BEGIN

class null_powersupply
 : public powersupply_operations {
 public:
	null_powersupply() {}
	virtual ~null_powersupply(){}
	operator powersupply_operations*() {
		return (powersupply_operations*)this;
	}

 public:
	virtual int32_t	get_voltage(void) {
		return 0;
	}
	virtual int32_t	get_voltage_max(void) {
		return 0;
	}
	virtual int32_t	get_voltage_min(void) {
		return 0;
	}
	virtual int32_t	get_current(void) {
		return 0;
	}
	virtual int32_t	get_battery_type(void) {
		return 0;
	}
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_NULL_POWERSUPPLY_H_ */
