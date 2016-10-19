/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_DEVICES_POWER_EV3DEV_POWERSUPPLY_H_
#define SHARAKU_DEVICES_POWER_EV3DEV_POWERSUPPLY_H_

#include <stdint.h>
#include <devices/sensor-api.hpp>
#include <devices/proc/ev3dev-proc-device.hpp>

NAMESPACE_SHARAKU_BEGIN

class ev3dev_powersupply
 : protected ev3dev_lego_powersupply
 , public powersupply_operations {
 private:
	ev3dev_powersupply() : ev3dev_lego_powersupply(1) {}

 public:
	ev3dev_powersupply(int interval_ms)
	: ev3dev_lego_powersupply(interval_ms) {
		_current	= 0;
		_voltage	= 0;
		_voltage_max	= 0;
		_voltage_min	= 0;

		connect();
	}
	virtual ~ev3dev_powersupply() {
		disconnect();
	}
	operator powersupply_operations*() {
		return (powersupply_operations *)this;
	}

 public:
	virtual int32_t	get_voltage(void) {
		return _voltage;
	}
	virtual int32_t	get_voltage_max(void) {
		return _voltage_max;
	}
	virtual int32_t	get_voltage_min(void) {
		return _voltage_min;
	}
	virtual int32_t	get_current(void) {
		return _current;
	}

 public:
	int32_t connect(void);

 protected:
	void __update(void);
	void __commit(void);
	void __io_end(PROC_IOTYPE type);

 private:
	int32_t		_current;
	int32_t		_voltage;
	int32_t		_voltage_max;
	int32_t		_voltage_min;
};

NAMESPACE_SHARAKU_END

#endif /* SHARAKU_DEVICES_POWER_EV3DEV_POWERSUPPLY_H_ */
