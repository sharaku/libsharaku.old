/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2015
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_SD_MOTORS_H_
#define _SHARAKU_DEVICES_SD_MOTORS_H_

#include <stdint.h>
#include <devices/api.hpp>

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
	virtual void pre_update(const float &interval);
	virtual void post_update(const float &interval);

 protected:
	// 設定値
	int32_t			_wheel_axle_length;
	int32_t			_wheel_length;

 private:
	sd_motors() {}
	void differential(int32_t steering);

	sharaku_usec_t		_time;		// 前回実行時の時間

	int32_t			_steer_sp;
	int32_t			_speed_sp;
	float			_steer;
	float			_speed;
	int32_t			_position;

	// 内部計算用中間値
	int32_t			_prev_sum;
	int32_t			_prev_deltas[3];
	float			_vL;
	float			_vR;
	float			_rho;
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_SM_MOTORS_H_ */
