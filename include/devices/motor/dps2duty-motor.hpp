﻿/* --
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

#ifndef SHARAKU_DEVICES_MOTOR_DPS2DUTY_MOTOR_H_
#define SHARAKU_DEVICES_MOTOR_DPS2DUTY_MOTOR_H_

#include <stdint.h>
#include <devices/motor-api.hpp>

NAMESPACE_SHARAKU_BEGIN

template<int32_t MAX_DPS>
class dps2duty_motor
 : virtual public motor_ctrl_operations
 , virtual public speed_motor_operations
 , virtual public duty_motor_operations
{
 public:
	// インターフェース接続(複数connect禁止)
	duty_motor_operations		*out_motor;

	operator motor_ctrl_operations*() {
		return (motor_ctrl_operations *)this;
	}
	operator speed_motor_operations*() {
		return (speed_motor_operations *)this;
	}
 public:
	// motor_ctrl_operations API
	virtual int32_t	get_position(void) {
		return out_motor->get_position();
	}
	virtual int32_t	set_position(int32_t pos) {
		return out_motor->set_position(pos);
	}
	virtual int32_t	get_stop_mode(void) {
		return out_motor->get_stop_mode();
	}
	virtual int32_t	set_stop_mode(stop_mode mode) {
		return out_motor->set_stop_mode(mode);
	}
	virtual int32_t	reset(void) {
		return out_motor->reset();
	}
	virtual int32_t	stop(void) {
		return out_motor->stop();
	}

	// speed_motor_operations API
	virtual int32_t	get_speed(void) {
		// かけてから割ることで、小数点演算せずに精度を維持
		return (out_motor->get_duty_cycle() * MAX_DPS) / 100;
	}
	virtual int32_t	set_speed_sp(int32_t dps) {
		// かけてから割ることで、小数点演算せずに精度を維持
		return out_motor->set_duty_cycle_sp((dps * 100) / MAX_DPS);
	}
	virtual int32_t	get_speed_sp(void) {
		return (out_motor->get_duty_cycle_sp() * MAX_DPS) / 100;
	}
	virtual int32_t	get_count_per_rot(void) {return -ENOTSUP;}
	virtual int32_t	set_speed_pid(float kp, float ki, float kd) {return -ENOTSUP;}

	// duty_motor_operations API
	virtual int32_t	get_duty_cycle(void) {
		return out_motor->get_duty_cycle();
	}
	virtual int32_t	set_duty_cycle_sp(int32_t cycle) {
		return out_motor->set_duty_cycle_sp(cycle);
	}
	virtual int32_t	get_duty_cycle_sp(void) {
		return out_motor->get_duty_cycle_sp();
	}
};

NAMESPACE_SHARAKU_END

#endif /* SHARAKU_DEVICES_MOTOR_DPS2DUTY_MOTOR_H_ */
