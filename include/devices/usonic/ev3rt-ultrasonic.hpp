/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_EV3RT_ULTRASONIC_H_
#define _SHARAKU_DEVICES_EV3RT_ULTRASONIC_H_

#include <stdint.h>
#include <sharaku/types.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <ev3api.h>
#include <devices/sensor-api.hpp>
#include <devices/ev3rt-device-def.h>

NAMESPACE_SHARAKU_BEGIN

class ev3rt_ultrasonic
 : public usonic_operations {
 private:
	ev3rt_ultrasonic() {}

 public:
	ev3rt_ultrasonic(sensor_port_t port) {
		_port = port;
		ev3_sensor_config(_port, ULTRASONIC_SENSOR);
	}
	virtual ~ev3rt_ultrasonic() {}

	// インタフェースへのアクセス
	operator usonic_operations*() {
		return (usonic_operations*)this;
	}

 public:
	virtual int32_t set_uint(usonic_operations::uint u) {
		return ENOTSUP;
	}
	virtual int32_t	get_dist(void) {
		return (int32_t)ev3_ultrasonic_sensor_get_distance(_port);
	}

 private:
	sensor_port_t _port;
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_EV3RT_ULTRASONIC_H_ */
