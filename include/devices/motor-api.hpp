/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
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
		STOPMODE_COAST,	// 電力をカットし、自然に止まる（惰性）
		STOPMODE_BRAKE,	// ブレーキをかけつつ、ゆっくり止まる
		STOPMODE_HOLD,	// 現在の位置を維持しようとする
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
