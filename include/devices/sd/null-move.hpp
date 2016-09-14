/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_NULL_MOVE_H_
#define _SHARAKU_DEVICES_NULL_MOVE_H_

#include <stdint.h>
#include <devices/moving-api.hpp>

NAMESPACE_SHARAKU_BEGIN

class null_move
 : public move_operations {
 public:
	null_move() {}
	virtual ~null_move(){}
	operator move_operations*() {
		return (move_operations*)this;
	}

 public:
	virtual int32_t	get_steer_sp(void) {
		return 0;
	}
	virtual int32_t	get_speed_sp(void) {
		return 0;
	}
	virtual int32_t	set_speed_steer_sp(int32_t dps, int32_t steer) {
		return 0;
	}
	virtual int32_t	get_speed(void) {
		return 0;
	}
	virtual int32_t	get_steer(void) {
		return 0;
	}
	virtual int32_t	get_position(void) {
		return 0;
	}
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_NULL_MOVE_H_ */
