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

// 1020 : 100 = 
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

 public:
	ev3way_balancer();
	virtual ~ev3way_balancer() {
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
	virtual int32_t	set_speed_steer_sp(int32_t dps, int32_t steer) {
		_speed_sp = dps * SHARAKU_EV3WAY_SPEED2DUTY;
		_steer_sp = steer;
		return 0;
	}

	// update_operations インタフェース
	virtual void update(const float &interval);

 private:

	sharaku_usec_t		_time;		// 前回実行時の時間

	int32_t			_steer_sp;
	int32_t			_speed_sp;
	float			_steer;
	float			_speed;
	int32_t			_position;

	// 内部計算用中間値
	int32_t			_prev_sum;
	int32_t			_prev_deltas[3];
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_SM_MOTORS_H_ */