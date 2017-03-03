/* --
 *
 * MIT License
 * 
 * Copyright (c) 2014-2016 Abe Takafumi
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

#ifndef SHARAKU_DEVICE_MOTOR_API_H_
#define SHARAKU_DEVICE_MOTOR_API_H_

#include <stdint.h>
#include <sharaku/types.h>

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------
// モータの制御インタフェース
class motor_ctrl_operations
{
 public:
	enum stop_mode {
		STOPMODE_COAST,		// 電力をカットし、自然に止まる（惰性）
		STOPMODE_BRAKE,		// ブレーキをかけつつ、ゆっくり止まる
		STOPMODE_HOLD,		// 現在の位置を維持しようとする
		STOPMODE_UNKNOWN,	// ????
	};

	virtual int32_t	get_position(void) {return -ENOTSUP;}
	virtual int32_t	set_position(int32_t pos) {return -ENOTSUP;}
	virtual int32_t	get_stop_mode(void) {return -ENOTSUP;}
	virtual int32_t	set_stop_mode(stop_mode mode) {return -ENOTSUP;}
	virtual int32_t	reset(void) {return -ENOTSUP;}
	virtual int32_t	stop(void) {return -ENOTSUP;}
};

//-----------------------------------------------------------------------------
// 速度指定のモータインタフェース

class speed_motor_operations
 : virtual public motor_ctrl_operations
{
 public:
	virtual int32_t	get_speed(void) {return -ENOTSUP;}
	virtual int32_t	set_speed_sp(int32_t dps) {return -ENOTSUP;}
	virtual int32_t	get_speed_sp(void) {return -ENOTSUP;}
	virtual int32_t	get_count_per_rot(void) {return -ENOTSUP;}
	virtual int32_t	set_speed_pid(float kp, float ki, float kd) {return -ENOTSUP;}
};

//-----------------------------------------------------------------------------
// 角度指定のモータインタフェース

class angle_motor_operations
 : virtual public motor_ctrl_operations
{
 public:
	virtual int32_t	get_position_sp(void) {return -ENOTSUP;}
	virtual int32_t	set_position_sp(int32_t r) {return -ENOTSUP;}
	virtual int32_t	add_position_sp(int32_t r) {return -ENOTSUP;}
	virtual int32_t	set_hold_pid(float kp, float ki, float kd) {return -ENOTSUP;}
};

//-----------------------------------------------------------------------------
// パワー指定のモータインタフェース

class duty_motor_operations
 : virtual public motor_ctrl_operations
{
 public:
	virtual int32_t	get_duty_cycle(void) {return -ENOTSUP;}
	virtual int32_t	set_duty_cycle_sp(int32_t cycle) {return -ENOTSUP;}
	virtual int32_t	get_duty_cycle_sp(void) {return -ENOTSUP;}
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_DEVICE_MOTOR_API_H_
