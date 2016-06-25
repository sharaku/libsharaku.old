/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2015
 * Author Abe Takafumi
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sharaku/prof.h>
#include <sharaku/log.h>
#include <devices/sd/ev3way/ev3way-balancer.hpp>
#include "balancer.h"

NAMESPACE_SHARAKU_BEGIN

ev3way_balancer::ev3way_balancer()
{
	sharaku_db_trace("balance_init", 0, 0, 0, 0, 0, 0);
	balance_init(); /* 倒立振子API初期化 */
}

void ev3way_balancer::update(const float &interval)
{
	signed char pwm_L, pwm_R; /* 左右モータPWM出力 */
	int gyro, volt;

	// 両輪の状態を取得し、位置と速度を算出する
	int32_t	left_pos	= -1 * out_duty_motor_l->get_position();
	int32_t	right_pos	= -1 * out_duty_motor_r->get_position();

	int32_t	pos_sum	= right_pos + left_pos;
	int32_t	delta	= pos_sum - _prev_sum;
	_position	+= delta;
	_speed	= (float)((delta +
			_prev_deltas[0] +
			_prev_deltas[1] +
			_prev_deltas[2]) / (4.0f * interval));
	sharaku_db_trace("interval=%u left_pos=%d right_pos=%d delta=%d _speed=%d _position=%d",
			 interval * 1000000, left_pos, right_pos, delta, _speed, _position);

	// 情報を更新
	_prev_sum	= pos_sum;
	_prev_deltas[2] = _prev_deltas[1];
	_prev_deltas[1] = _prev_deltas[0];
	_prev_deltas[0] = delta;

	gyro = -1 * in_gyro->get_angle();
	volt = in_power->get_voltage();
	sharaku_db_trace("_speed_sp=%d _steer_sp=%d gyro=%d left_pos=%d right_pos=%d volt=%d",
			 _speed_sp, _steer_sp, gyro, left_pos, right_pos, volt);
	balance_control((float)_speed_sp,
			(float)_steer_sp,
			(float)gyro,
			(float)0,
			(float)left_pos,
			(float)right_pos,
			(float)volt,
			(signed char*)&pwm_L,
			(signed char*)&pwm_R,
			interval);

	sharaku_db_trace("pwm_L=%d pwm_R=%d", pwm_L, pwm_R, 0, 0, 0, 0);
	out_duty_motor_l->set_duty_cycle_sp(-1 * pwm_L);
	out_duty_motor_r->set_duty_cycle_sp(-1 * pwm_R);
}

NAMESPACE_SHARAKU_END
