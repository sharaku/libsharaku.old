/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_NULL_ODOMETRY_H_
#define _SHARAKU_DEVICES_NULL_ODOMETRY_H_

#include <stdint.h>
#include <devices/odmetry-api.hpp>

NAMESPACE_SHARAKU_BEGIN

class null_odmetry
 : public odmetry_operations {
 public:
	null_odmetry() {
		pos(0.0f, 0.0f, 0.0f)
		rot(0.0f, 0.0f, 0.0f)
	}
	virtual ~null_odmetry(){}
	operator odmetry_operations*() {
		return (odmetry_operations*)this;
	}

 public:
	virtual const position3& get_position(void) {
		return pos;
	}
	virtual const rotation3& get_rotation(void) {
		return rot;
	}
	virtual const int32_t get_dist(void) {
		return 0;
	}
	virtual int32_t reset(void) {
		return 0;
	}

 private:
	position3	pos;
	rotation3	rot;
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_NULL_ODOMETRY_H_ */
