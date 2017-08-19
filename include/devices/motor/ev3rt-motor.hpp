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

#ifndef _SHARAKU_DEVICE_EV3RT_MOTOR_H_
#define _SHARAKU_DEVICE_EV3RT_MOTOR_H_

#include <stdint.h>
#include <sharaku/types.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <ev3api.h>
#include <devices/motor-api.hpp>
#include <devices/ev3rt-device-def.h>

NAMESPACE_SHARAKU_BEGIN

class ev3rt_motor
 : virtual public motor_ctrl_operations
 , virtual public speed_motor_operations
 , virtual public angle_motor_operations
 , virtual public duty_motor_operations
{
 public:
	ev3rt_motor(motor_port_t port) {
		_port = port;
		_position_sp = 0;
		ev3_motor_config(_port, LARGE_MOTOR);
		ev3_motor_reset_counts(_port);
	}
	virtual ~ev3rt_motor() {
		ev3_motor_stop(_port, true);
	}
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
		return ev3_motor_get_counts(_port);
	}
	virtual int32_t	set_position(int32_t pos) {
		return ev3_motor_reset_counts(_port);
	}
	virtual int32_t	get_stop_mode(void) {
		ev3_motor_stop(_port, true);
		return 0;
	}
	virtual int32_t	set_stop_mode(stop_mode mode) {
		switch (mode) {
		case STOPMODE_BRAKE:
			ev3_motor_stop(_port, true);
			break;
		default:
			ev3_motor_stop(_port, false);
			break;
		}
		return 0;
	}

	virtual int32_t	get_speed(void) {
		return 0;
	}
	virtual int32_t	set_speed_sp(int32_t rpm) {
		return ENOTSUP;
	}
	virtual int32_t	get_speed_sp(void) {
		return 0;
	}
	virtual int32_t	get_count_per_rot(void) {
		return ENOTSUP;
	}
	virtual int32_t	set_speed_pid(float kp,float ki, float kd) {
		return ENOTSUP;
	}

	virtual int32_t	get_position_sp(void) {
		return ev3_motor_get_counts(_port);
	}
	virtual int32_t	set_position_sp(int32_t r) {
		_position_sp = r;
		return ev3_motor_rotate(_port, _position_sp, 100, true);
	}
	virtual int32_t	add_position_sp(int32_t r) {
		_position_sp += r;
		return ev3_motor_rotate(_port, _position_sp, 50, true);
	}
	virtual int32_t	set_hold_pid(float kp,float ki, float kd) {
		return ENOTSUP;
	}

	virtual int32_t	get_duty_cycle(void) {
		return ev3_motor_get_power(_port);
	}
	virtual int32_t	set_duty_cycle_sp(int32_t cycle) {
		if (cycle > 100) {
			cycle = 100;
		}
		if (cycle < -100) {
			cycle = -100;
		}
		return ev3_motor_set_power(_port, cycle);
	}
	virtual int32_t	get_duty_cycle_sp(void) {
		return ev3_motor_get_power(_port);
	}

 private:
	motor_port_t	_port;
	int32_t		_position_sp;
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICE_EV3RT_MOTOR_H_ */
