/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_NULL_MOTOR_H_
#define _SHARAKU_DEVICES_NULL_MOTOR_H_

#include <stdint.h>
#include <devices/motor-api.hpp>

NAMESPACE_SHARAKU_BEGIN

class null_motor
 : virtual public motor_ctrl_operations
 , virtual public speed_motor_operations
 , virtual public angle_motor_operations
 , virtual public duty_motor_operations
{
 public:
	null_motor() {}
	virtual ~null_motor() {}

	// インタフェースへのアクセス
	operator motor_ctrl_operations*() {
		return (motor_ctrl_operations*)this;
	}
	operator speed_motor_operations*() {
		return (speed_motor_operations*)this;
	}
	operator angle_motor_operations*() {
		return (angle_motor_operations*)this;
	}
	operator duty_motor_operations*() {
		return (duty_motor_operations*)this;
	}

 public:
	virtual int32_t	get_position(void) {
		return 0;
	}
	virtual int32_t	set_position(int32_t pos) {
		return 0;
	}
	virtual int32_t	get_stop_mode(void) {
		return 0;
	}
	virtual int32_t	set_stop_mode(stop_mode mode) {
		return 0;
	}
	virtual int32_t	set_speed_pid(float kp, float ki, float kd) {
		return 0;
	}
	virtual int32_t	set_hold_pid(float kp, float ki, float kd) {
		return 0;
	}

	virtual int32_t	get_speed(void) {
		return 0;
	}
	virtual int32_t	set_speed_sp(int32_t rpm) {
		return 0;
	}
	virtual int32_t	get_speed_sp(void) {
		return 0;
	}
	virtual int32_t	get_count_per_rot(void) {
		return 0;
	}

	virtual int32_t	get_position_sp(void) {
		return 0;
	}

	virtual int32_t	set_position_sp(int32_t r) {
		return 0;
	}

	virtual int32_t	add_position_sp(int32_t r) {
		return 0;
	}


	virtual int32_t	get_duty_cycle(void) {
		return 0;
	}
	virtual int32_t	set_duty_cycle_sp(int32_t cycle) {
		return 0;
	}
	virtual int32_t	get_duty_cycle_sp(void)  {
		return 0;
	}
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_NULL_MOTOR_H_ */
