/* --
 *
 * MIT License
 * 
 * Copyright (c) 2014-2015 Abe Takafumi
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

#ifndef _SHARAKU_DEVICES_SD_MOTORS_H_
#define _SHARAKU_DEVICES_SD_MOTORS_H_

#include <stdint.h>
#include <sharaku/utime.h>
#include <devices/update-api.hpp>
#include <devices/moving-api.hpp>
#include <devices/sensor-api.hpp>
#include <devices/motor-api.hpp>
#include <devices/connection_interface.hpp>
#include <sharaku/digital-filter.hpp>

NAMESPACE_SHARAKU_BEGIN

// 1020 : 100
#define SHARAKU_EV3WAY_DUTY2SPEED	(10.2f)
#define SHARAKU_EV3WAY_SPEED2DUTY	(0.09803921f)

//-----------------------------------------------------------------------------
// IMoveインタフェースを2つのモータと1つのステアリングへ振り分けるモジュール
class ev3way_balancer
 : protected move_operations
 , protected update_operations
{
 public:
	// インターフェース接続
	connection_interface<powersupply_operations>	in_power;
	connection_interface<gyro_operations>		in_gyro;
	connection_interface<duty_motor_operations>	out_duty_motor_l;
	connection_interface<duty_motor_operations>	out_duty_motor_r;

	operator move_operations*() { return (move_operations*)this;}
	operator update_operations*() { return (update_operations*)this;}

	// reset
	virtual int32_t	reset(void);

 public:
	ev3way_balancer();
	virtual ~ev3way_balancer() {
	}
	virtual int32_t	set_speed_steer_sp(int32_t dps, int32_t steer) {
		_speed_sp = dps;
		_steer_sp = steer;
		return 0;
	}

	virtual float get_interval(void) {
		return _interval_raw;
	}
	virtual float get_exec_period(void) {
		return _interval;
	}

	// sm_move_op インタフェース
	virtual void set_max_dps(int32_t dps) {
		_max_dps = dps;
	}
	virtual void balancer_on(void) {
		reset();
		_onoff = true;
	}
	virtual void balancer_off(void) {
		_onoff = false;
	}
	// move_operations インタフェース
	virtual int32_t	get_steer_sp(void) {
		return _steer_sp;
	}
	virtual int32_t	get_speed_sp(void) {
		return _speed_sp;
	}
	virtual int32_t	get_steer(void) {
		return _steer;
	}
	virtual int32_t	get_speed(void) {
		return _speed;
	}
	virtual int32_t	get_position(void) {
		return _position;
	}
	virtual int32_t	set_speed_sp(int32_t dps) {
		_speed_sp = dps;
		return 0;
	}
	virtual int32_t	set_steer_sp(int32_t steer) {
		_steer_sp = steer;
		return 0;
	}
	// update_operations インタフェース
	virtual int32_t update(const float &interval, uint32_t retry_cnt);

 private:

	sharaku_usec_t		_time;		// 前回実行時の時間
	bool			_onoff;		// 機能On/Off

	int32_t			_max_dps;
	int32_t			_steer_sp;
	int32_t			_speed_sp;
	float			_steer;
	float			_speed;
	int32_t			_position;

	// 内部計算用中間値
	int32_t			_prev_sum;
	int32_t			_prev_deltas[3];
	low_pass_filter		_interval;
	float			_interval_raw;
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_SM_MOTORS_H_ */
