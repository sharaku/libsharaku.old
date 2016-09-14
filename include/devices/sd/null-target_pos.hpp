/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_NULL_ODOMETRY_H_
#define _SHARAKU_DEVICES_NULL_ODOMETRY_H_

#include <stdint.h>
#include <devices/moving-api.hpp>

NAMESPACE_SHARAKU_BEGIN

class null_target_pos
 : public target_pos_operations {
 public:
	null_target_pos() {
		pos(0.0f, 0.0f, 0.0f)
		rot(0.0f, 0.0f, 0.0f)
	}
	virtual ~null_target_pos(){}
	operator target_pos_operations*() {
		return (target_pos_operations*)this;
	}

 public:
	virtual int32_t	set_position_sp(position3&) {
		return 0;
	}
	virtual const position3& get_position_sp(void) {
		return pos;
	}
	virtual int32_t	set_rotation_sp(rotation3&) {
		return 0;
	}
	virtual const rotation3& get_rotation_sp(void){
		return rot;
	}

 private:
	position3	pos;
	rotation3	rot;
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_NULL_ODOMETRY_H_ */
