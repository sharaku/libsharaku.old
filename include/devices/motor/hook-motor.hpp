/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICE_HOOK_MOTOR_H_
#define _SHARAKU_DEVICE_HOOK_MOTOR_H_

#include <stdint.h>
#include <devices/motor-api.hpp>
#include <devices/connection_interface.hpp>

NAMESPACE_SHARAKU_BEGIN

class hook_motor
 : virtual public motor_ctrl_operations
 , virtual public speed_motor_operations
 , virtual public angle_motor_operations
 , virtual public duty_motor_operations
{
 public:
	hook_motor() {}
	virtual ~hook_motor() {}
	// インターフェース接続(複数connect禁止)
	connection_interface<speed_motor_operations> out_speed;
	connection_interface<angle_motor_operations> out_angle;
	connection_interface<duty_motor_operations> out_duty;

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
	virtual int32_t	get_position(void);
	virtual int32_t	set_position(int32_t pos);
	virtual int32_t	get_stop_mode(void);
	virtual int32_t	set_stop_mode(stop_mode mode);

	virtual int32_t	get_speed(void);
	virtual int32_t	set_speed_sp(int32_t rpm);
	virtual int32_t	get_speed_sp(void);
	virtual int32_t	get_count_per_rot(void);
	virtual int32_t	set_speed_pid(float kp,float ki, float kd);

	virtual int32_t	get_position_sp(void);
	virtual int32_t	set_position_sp(int32_t r);
	virtual int32_t	add_position_sp(int32_t r);
	virtual int32_t	set_hold_pid(float kp,float ki, float kd);


	virtual int32_t	get_duty_cycle(void);
	virtual int32_t	set_duty_cycle_sp(int32_t cycle);
	virtual int32_t	get_duty_cycle_sp(void);
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICE_HOOK_MOTOR_H_ */
