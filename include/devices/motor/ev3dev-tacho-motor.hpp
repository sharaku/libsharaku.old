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

#ifndef SHARAKU_DEVICES_MOTOR_EV3DEV_TACHO_MOTOR_H_
#define SHARAKU_DEVICES_MOTOR_EV3DEV_TACHO_MOTOR_H_

#include <stdint.h>
#include <devices/motor-api.hpp>
#include <devices/proc/ev3dev-proc-device.hpp>

NAMESPACE_SHARAKU_BEGIN

class ev3dev_tacho_motor
 : protected ev3dev_lego_tacho_motor
 , virtual public motor_ctrl_operations
 , virtual public speed_motor_operations
 , virtual public angle_motor_operations
 , virtual public duty_motor_operations
{
 private:
	ev3dev_tacho_motor() : ev3dev_lego_tacho_motor(1) {}

 public:
	enum motor_mode {
		MOTOR_MODE_SPEED,
		MOTOR_MODE_ANGLE,
		MOTOR_MODE_DUTY,
		MOTOR_MODE_STOP,
		MOTOR_MODE_UNKNOWN,
	};

	ev3dev_tacho_motor(const char *port, int interval_ms, motor_mode mode)
	 : ev3dev_lego_tacho_motor(interval_ms) {
		_motor_mode	= MOTOR_MODE_STOP;
		_motor_mode_sp	= MOTOR_MODE_UNKNOWN;
		_flag		= FLAG_RESET;
		// motor_ctrl_operations API
		_stop_mode	= STOPMODE_UNKNOWN;
		_stop_mode_sp	= STOPMODE_BRAKE;
		_position_base	= 0;
		_position	= 0;
		// speed_motor_operations API
		_count_per_rot	= 0;
		_speed		= 0;
		_speed_sp	= 0;
		_speed_pid_kd	= 0;
		_speed_pid_ki	= 0;
		_speed_pid_kp	= 0;
		// angle_motor_operations API
		_position_sp	= 0;
		_hold_pid_kd	= 0;
		_hold_pid_ki	= 0;
		_hold_pid_kp	= 0;
		// duty_motor_operations API
		_duty_cycle	= 0;
		_duty_cycle_sp	= 0;

		connect(port);
		set_motor_mode(mode);
	}
	virtual ~ev3dev_tacho_motor() {
		disconnect();
	}

	// インタフェースへのアクセス
	operator motor_ctrl_operations*() {
		return (motor_ctrl_operations *)this;
	}
	operator speed_motor_operations*() {
		return (speed_motor_operations *)this;
	}
	operator angle_motor_operations*() {
		return (angle_motor_operations *)this;
	}
	operator duty_motor_operations*() {
		return (duty_motor_operations *)this;
	}
	operator device_update_operations*() {
		return (device_update_operations *)((ev3dev_lego_tacho_motor*)this);
	}
	int set_motor_mode(motor_mode mode) {
		_motor_mode_sp = mode;
		return 0;
	}
 public:
	// motor_ctrl_operations API
	virtual int32_t	get_position(void) {
		return _position;
	}
	virtual int32_t	set_position(int32_t pos) {
		_position = pos;
		return _position;
	}
	virtual int32_t	get_stop_mode(void) {
		return _stop_mode;
	}
	virtual int32_t	set_stop_mode(stop_mode mode) {
		_stop_mode_sp = mode;
		return _stop_mode;
	}
	virtual int32_t	reset(void) {
		_flag |= FLAG_RESET;
		return 0;
	}
	virtual int32_t	stop(void) {
		_flag |= FLAG_STOP;
		return 0;
	}

	// speed_motor_operations API
	virtual int32_t	get_speed(void) {
		return _speed;
	}
	virtual int32_t	set_speed_sp(int32_t dps) {
		_speed_sp = dps;
		return _speed_sp;
	}
	virtual int32_t	get_speed_sp(void) {
		return _speed_sp;
	}
	virtual int32_t	get_count_per_rot(void) {
		return _count_per_rot;
	}
	virtual int32_t	set_speed_pid(float kp, float ki, float kd) {
		_speed_pid_kd = kd;
		_speed_pid_ki = ki;
		_speed_pid_kp = kp;
		return 0;
	}

	// angle_motor_operations API
	virtual int32_t	get_position_sp(void) {
		return _position_sp;
	}
	virtual int32_t	set_position_sp(int32_t r) {
		_position_sp = r;
		return _position_sp;
	}
	virtual int32_t	add_position_sp(int32_t r) {
		_position_sp = _position + r;
		return _position_sp;
	}
	virtual int32_t	set_hold_pid(float kp, float ki, float kd) {
		_hold_pid_kd = kd;
		_hold_pid_ki = ki;
		_hold_pid_kp = kp;
		return 0;
	}

	// duty_motor_operations API
	virtual int32_t	get_duty_cycle(void) {
		return _duty_cycle;
	}
	virtual int32_t	set_duty_cycle_sp(int32_t cycle) {
		_duty_cycle_sp = cycle;
		return _duty_cycle_sp;
	}
	virtual int32_t	get_duty_cycle_sp(void) {
		return _duty_cycle_sp;
	}

 public:
	int32_t connect(const char* port);

 protected:
	void __update(void);
	void __commit(void);
	void __io_end(PROC_IOTYPE type);

 private:
 	enum {
		FLAG_RESET = 0x00000001,
		FLAG_STOP = 0x00000002,
	};
	motor_mode		_motor_mode;
	uint32_t		_flag;
	motor_mode		_motor_mode_sp;
	// motor_ctrl_operations API
	stop_mode		_stop_mode;
	stop_mode		_stop_mode_sp;
	int32_t			_position_base;
	int32_t			_position;
	// speed_motor_operations API
	int32_t			_count_per_rot;
	int32_t			_speed;
	int32_t			_speed_sp;
	int32_t			_speed_pid_kd;
	int32_t			_speed_pid_ki;
	int32_t			_speed_pid_kp;
	// angle_motor_operations API
	int32_t			_position_sp;
	int32_t			_hold_pid_kd;
	int32_t			_hold_pid_ki;
	int32_t			_hold_pid_kp;
	// duty_motor_operations API
	int32_t			_duty_cycle;
	int32_t			_duty_cycle_sp;
};

NAMESPACE_SHARAKU_END

#endif /* SHARAKU_DEVICES_MOTOR_EV3DEV_TACHO_MOTOR_H_ */
