/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2015
 * Author Abe Takafumi
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
#include <devices/connection_interface.hpp>

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------
// IMoveインタフェースを2つのモータと1つのステアリングへ振り分けるモジュール
class sd_motors
 : protected move_operations
 , protected update_operations
{
 public:
	// インターフェース接続
	connection_interface<speed_motor_operations>	out_speed_motor_l;
	connection_interface<speed_motor_operations>	out_speed_motor_r;
	connection_interface<device_update_operations>	out_device_update_l;
	connection_interface<device_update_operations>	out_device_update_r;

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

	sharaku_usec_t		_time;		// 前回実行時の時間

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
