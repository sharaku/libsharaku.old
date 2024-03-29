﻿/* --
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
 * SOFTWARE.
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sharaku/prof.h>
#include <sharaku/log.h>
#include <sharaku/debug.h>
#include <devices/sd/ev3way/ev3way-balancer.hpp>
#include "balancer.h"

float EXEC_PERIOD;

NAMESPACE_SHARAKU_BEGIN

ev3way_balancer::ev3way_balancer()
 : _interval(0.4)
{
	_max_dps = 1000;
	sharaku_db_trace("balance_init", 0, 0, 0, 0, 0, 0);
	reset();
	_interval = 0.00400000000F;

	sharaku_prof_init(&_prof_interval, "ev3way_balancer::interval");
	sharaku_prof_init(&_prof_update_process, "ev3way_balancer::process");
	debug_code(sharaku_prof_regist(&_prof_interval));
	debug_code(sharaku_prof_regist(&_prof_update_process));
}

int32_t
ev3way_balancer::reset(void)
{
	sharaku_db_trace("reset", 0, 0, 0, 0, 0, 0);
	balance_init(); /* 倒立振子API初期化 */
	return 0;
}

int32_t ev3way_balancer::update(const float &interval, uint32_t retry_cnt)
{
	signed char pwm_L = 0, pwm_R = 0; /* 左右モータPWM出力 */

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

	EXEC_PERIOD = _interval + interval;
	_interval_raw = interval;
	sharaku_db_trace("interval=%u left_pos=%d right_pos=%d delta=%d _speed=%d _position=%d",
			 EXEC_PERIOD * 1000000, left_pos, right_pos, delta, _speed, _position);

	// 情報を更新
	_prev_sum	= pos_sum;
	_prev_deltas[2] = _prev_deltas[1];
	_prev_deltas[1] = _prev_deltas[0];
	_prev_deltas[0] = delta;

	if (_onoff) {
		int32_t motor_ang_l, motor_ang_r;
		int32_t gyro, volt;

		motor_ang_l = out_duty_motor_l->get_position();
		motor_ang_r = out_duty_motor_r->get_position();
		gyro = in_gyro->get_rate();
		volt = in_power->get_voltage();

		balance_control((float)(_speed_sp * 100) / _max_dps,
				(float)_steer_sp,
				(float)gyro,
				(float)0,
				(float)motor_ang_l,
				(float)motor_ang_r,
				(float)volt,
				(signed char*)&pwm_L,
				(signed char*)&pwm_R);
	}

	sharaku_db_trace("pwm_L=%d pwm_R=%d", pwm_L, pwm_R, 0, 0, 0, 0);
	out_duty_motor_l->set_duty_cycle_sp(pwm_L);
	out_duty_motor_r->set_duty_cycle_sp(pwm_R);

	return 0;
}

NAMESPACE_SHARAKU_END
