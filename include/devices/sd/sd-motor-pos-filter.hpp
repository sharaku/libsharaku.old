/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_UV_MODULES_MOTOR_POS_FILTER_H_
#define SHARAKU_UV_MODULES_MOTOR_POS_FILTER_H_

#include <stdint.h>
#include <devices/motor-api.hpp>
#include <sharaku/digital-filter.hpp>

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------

class sd_motor_pos_filter
 : virtual public speed_motor_operations
{
 public:
	// インターフェース接続(複数connect禁止)
	connection_interface<speed_motor_operations>	out_motor;

	operator speed_motor_operations* () {
		return (speed_motor_operations*) this;
	}

 public:
	sd_motor_pos_filter() : _lowpass(1.0f){
	}

	void set_lowpass(float filter) {
		_lowpass.set(filter);
		_lowpass.clear();
	}

	int32_t	get_speed(void) {
		return out_motor->get_speed();
	}
	int32_t	set_speed_sp(int32_t rpm) {
		return out_motor->set_speed_sp(rpm);
	}
	int32_t	get_speed_sp(void) {
		return out_motor->get_speed_sp();
	}
	int32_t	get_count_per_rot(void) {
		return out_motor->get_count_per_rot();
	}
	int32_t	get_position(void) {
		int32_t pos = (int32_t)(_lowpass + out_motor->get_position());
		return pos;
	}
	int32_t	set_position(int32_t pos) {
		return out_motor->set_position(pos);
	}
	int32_t get_stop_mode(void) {
		return out_motor->get_stop_mode();
	}
	int32_t set_stop_mode(stop_mode mode) {
		return out_motor->set_stop_mode(mode);
	}
	int32_t	set_speed_pid(int kp,int ki, int kd) {
		return out_motor->set_speed_pid(kp, ki, kd);
	}
private:
	low_pass_filter _lowpass;
};


NAMESPACE_SHARAKU_END

#endif // SHARAKU_UV_MODULES_GEAR_RATIO_H_
