/* --
 *
 * MIT License
 * 
 * Copyright (c) 2017 Abe Takafumi
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

#ifndef SHARAKU_SIMURATON_MOTOR_H_
#define SHARAKU_SIMURATON_MOTOR_H_

#include <stdint.h>
#include <sharaku/pid.hpp>
#include <devices/motor-api.hpp>

NAMESPACE_SHARAKU_BEGIN

namespace simuration {

class motor_simuration
{
public:
	motor_simuration(int rpm);
	virtual void update(float second);
	void set_rpm(int32_t rpm);

	int32_t set_position(int32_t position) {
		return _position = (float)position;
	}

	int32_t set_duty_cycle(int32_t duty_cycle) {
		return _duty_cycle = duty_cycle;
	}

	int32_t set_duty_cycle_sp(int32_t duty_cycle_sp) {
		if (duty_cycle_sp > 100) {
			_duty_cycle_sp = 100;
		} else if (duty_cycle_sp < -100) {
			_duty_cycle_sp = -100;
		} else {
			_duty_cycle_sp = duty_cycle_sp;
		}
		return _duty_cycle_sp;
	}

	int32_t get_position() {
		return (int32_t)_position;
	}

	int32_t get_duty_cycle() {
		return _duty_cycle;
	}

	int32_t get_duty_cycle_sp() {
		return _duty_cycle_sp;
	}
private:
	float		_position;	// 現在の回転角度
	int32_t		_duty_cycle;	// 現在のduty_cycle値
	int32_t		_duty_cycle_sp;	// duty_cycle指示値
	float		_duty2dps[101];	// duty_cycle値に対しての加算角度テーブル
};

class tacho_motor_simuration
 : public motor_simuration
{
public:
	tacho_motor_simuration(int rpm);
	virtual void update(float second);

	void set_position_mode(int32_t onoff) {
		_position_mode = onoff;
		return;
	}
	int32_t get_position_sp(void) {
		return _position_sp;
	}
	int32_t set_position_sp(int32_t position_sp) {
		return _position_sp = position_sp;
	}
	void set_hold_pid(float kp, float ki, float kd) {
		_hold_pid.set_pid(kp, ki, kd);
	}

	void set_speed_mode(int32_t onoff) {
		_speed_mode = onoff;
		return;
	}
	int32_t get_speed(void) {
		return _speed_sp;
	}
	int32_t get_speed_sp(void) {
		return _speed_sp;
	}
	int32_t set_speed_sp(int32_t position_sp) {
		return _speed_sp = position_sp;
	}
	void set_speed_pid(float kp, float ki, float kd) {
		_speed_pid.set_pid(kp, ki, kd);
	}
private:
	tacho_motor_simuration();
	int		_position_mode;
	float		_position_sp;
	pid		_hold_pid;

	int		_speed_mode;
	float		_speed;
	float		_targetpos;
	float		_speed_sp;
	pid		_speed_pid;
};

};

class simuration_motor
 : private simuration::tacho_motor_simuration
 , virtual public motor_ctrl_operations
 , virtual public speed_motor_operations
 , virtual public angle_motor_operations
 , virtual public duty_motor_operations
{
public:
	enum motor_mode {
		MOTOR_MODE_SPEED,
		MOTOR_MODE_ANGLE,
		MOTOR_MODE_DUTY,
		MOTOR_MODE_STOP
	};

	simuration_motor()
	 : simuration::tacho_motor_simuration(0) {}
	virtual void update();

	void set_rpm(int rpm) {
		simuration::motor_simuration::set_rpm(rpm);
	}

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

	int set_motor_mode(motor_mode mode) {
		switch (mode) {
		case MOTOR_MODE_SPEED:
			simuration::tacho_motor_simuration
					::set_position_mode(0);
			simuration::tacho_motor_simuration
					::set_speed_mode(1);
			break;
		case MOTOR_MODE_ANGLE:
			simuration::tacho_motor_simuration
					::set_position_mode(1);
			simuration::tacho_motor_simuration
					::set_speed_mode(0);
			break;
		case MOTOR_MODE_DUTY:
			simuration::tacho_motor_simuration
					::set_position_mode(0);
			simuration::tacho_motor_simuration
					::set_speed_mode(0);
			break;
		case MOTOR_MODE_STOP:
			stop();
			break;
		}
		return 0;
	}

public:
	// motor_ctrl_operations
	virtual int32_t set_position(int32_t position) {
		return simuration::tacho_motor_simuration
				::set_position(position);
	}
	virtual int32_t get_position() {
		return simuration::tacho_motor_simuration
				::get_position();
	}
	virtual int32_t	get_stop_mode(void) {
		return STOPMODE_HOLD;
	}
	virtual int32_t	set_stop_mode(stop_mode mode) {
		return STOPMODE_HOLD;
	}
	virtual int32_t	reset(void) {
		simuration::tacho_motor_simuration
				::set_position(0);
		return 0;
	}
	virtual int32_t	stop(void) {
		simuration::tacho_motor_simuration
				::set_position(0);
		return 0;
	}

public:
	// duty_motor_operations
	virtual int32_t set_duty_cycle_sp(int32_t duty_cycle_sp) {
		return simuration::tacho_motor_simuration
				::set_duty_cycle_sp(duty_cycle_sp);
	}
	virtual int32_t get_duty_cycle(void) {
		return simuration::tacho_motor_simuration
				::get_duty_cycle();
	}
	virtual int32_t get_duty_cycle_sp(void) {
		return simuration::tacho_motor_simuration
				::get_duty_cycle_sp();
	}

public:
	// angle_motor_operations
	virtual int32_t get_position_sp(void) {
		return simuration::tacho_motor_simuration
				::get_position_sp();
	}
	virtual int32_t set_position_sp(int32_t position_sp) {
		return simuration::tacho_motor_simuration
				::set_position_sp(position_sp);
	}
	virtual int32_t add_position_sp(int32_t position_sp) {
		int32_t position_sp_ = get_position_sp()
					+ position_sp;
		return simuration::tacho_motor_simuration
				::set_position_sp(position_sp_);
	}
	virtual int32_t	set_hold_pid(float kp, float ki, float kd) {
		simuration::tacho_motor_simuration
				::set_hold_pid(kp, ki, kd);
		return 0;
	}

public:
	// speed_motor_operations
	virtual int32_t get_speed(void) {
		return simuration::tacho_motor_simuration
				::get_speed();
	}
	virtual int32_t get_speed_sp(void) {
		return simuration::tacho_motor_simuration
				::get_speed_sp();
	}
	virtual int32_t set_speed_sp(int32_t position_sp) {
		return simuration::tacho_motor_simuration
				::set_speed_sp(position_sp);
	}
	virtual int32_t	get_count_per_rot(void) {
		return 360;
	}
	virtual int32_t	set_speed_pid(float kp, float ki, float kd) {
		simuration::tacho_motor_simuration
				::set_speed_pid(kp, ki, kd);
		return 0;
	}

private:
	motor_mode	_motor_mode_sp;
};

NAMESPACE_SHARAKU_END

#endif
