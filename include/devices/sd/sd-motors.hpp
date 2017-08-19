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
#include <sharaku/pid.hpp>
#include <devices/update-api.hpp>
#include <devices/moving-api.hpp>
#include <devices/motor-api.hpp>

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------
// IMoveインタフェースを2つのモータと1つのステアリングへ振り分けるモジュール
class sd_motors
 : protected move_operations
 , protected update_operations
{
 public:
	// インターフェース接続
	speed_motor_operations		*out_speed_motor_l;
	speed_motor_operations		*out_speed_motor_r;
	device_update_operations	*out_device_update_l;
	device_update_operations	*out_device_update_r;

	operator move_operations*() { return (move_operations*)this;}
	operator update_operations*() { return (update_operations*)this;}

 public:
	sd_motors(int32_t wheel_axle_length, int32_t wheel_length);
	virtual ~sd_motors() {
	}

	// 左右モータの補正を有効にする。
	// 有効にすると反応速度は一瞬遅れるが電圧やモータ特性に左右されなくなる。
	// 無効にすると反応速度は速いが、電圧やモータ特性に左右される。
	virtual void auto_correction_on(void) {
		_auto_correction = 1;
		_pos_l_motor = 0;
		_pos_r_motor = 0;
	}
	virtual void auto_correction_off(void) {
		_auto_correction = 0;
	}

	// move_operations インタフェース
	virtual int32_t	get_steer_sp(void) {
		return _steer_sp;
	}
	virtual int32_t	get_speed_sp(void) {
		return _speed_sp;
	}
	virtual int32_t	get_speed(void) {
		return _speed;
	}
	virtual int32_t	get_position(void) {
		return _position;
	}
	virtual int32_t	set_speed_sp(int32_t dps);
	virtual int32_t	set_steer_sp(int32_t steer_deg);
	virtual int32_t	get_steer_deg(void) {
		return _steer;
	}
	virtual int32_t	get_steer_rho(void) {
		return (int32_t)_rho;
	}

	// update_operations インタフェース
	virtual int32_t pre_update(const float &interval, uint32_t retry_cnt);
	virtual int32_t post_update(const float &interval, uint32_t retry_cnt);

 protected:
	// 設定値
	int32_t			_wheel_axle_length;
	int32_t			_wheel_length;

 private:
	sd_motors()
	 : _pid_l_motor(1.0f, 0.0f, 0.0f)
	 , _pid_r_motor(1.0f, 0.0f, 0.0f){}
	void differential(int32_t steering);

	int32_t			_steer_sp;
	int32_t			_speed_sp;
	float			_steer;
	float			_speed;
	int32_t			_position;
	int32_t			_auto_correction;

	// 内部計算用中間値
	int32_t			_prev_sum;
	int32_t			_prev_deltas[3];
	float			_vL;
	float			_vR;
	float			_rho;

	// 左右モータ補正
	pid			_pid_l_motor;
	int32_t			_pos_l_motor;
	int32_t			_dps_l_motor;
	pid			_pid_r_motor;
	int32_t			_pos_r_motor;
	int32_t			_dps_r_motor;
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_SM_MOTORS_H_ */
