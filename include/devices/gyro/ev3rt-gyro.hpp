/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef __SHARAKU_DEVICES_EV3RT_GYRO_H_
#define __SHARAKU_DEVICES_EV3RT_GYRO_H_

#include <stdint.h>
#include <sharaku/types.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <ev3api.h>
#include <devices/sensor-api.hpp>
#include <devices/ev3rt-device-def.h>

NAMESPACE_SHARAKU_BEGIN

class ev3rt_gyro
 : public gyro_operations {
 public:
	ev3rt_gyro(sensor_port_t port) {
		_port = port;
		ev3_sensor_config(_port, GYRO_SENSOR);
	}
	virtual ~ev3rt_gyro() {}

	// インタフェースへのアクセス
	operator gyro_operations*() {
		return (gyro_operations *)this;
	}

 public:
	virtual int32_t	get_angle(void){
		return (int32_t)ev3_gyro_sensor_get_angle(_port);
	}
	virtual int32_t	get_rate(void){
		return (int32_t)ev3_gyro_sensor_get_rate(_port);
	}
	virtual int32_t	reset(void){
		if (ev3_gyro_sensor_reset(_port) == E_OK) {
			(void)ev3_gyro_sensor_get_rate(_port);
			return 0;
		} else {
			(void)ev3_gyro_sensor_get_rate(_port);
			return ENOTSUP;
		}
	}
 private:
	sensor_port_t	_port;
};

NAMESPACE_SHARAKU_END

#endif /* __SHARAKU_DEVICES_EV3RT_GYRO_H_ */
