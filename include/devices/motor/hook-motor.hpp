/* --
 *
 * MIT License
 * 
 * Copyright (c) 2016 Abe Takafumi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. *
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
