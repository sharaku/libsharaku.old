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
 * SOFTWARE.
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <devices/sd/sd-motors.hpp>
#include <sharaku/prof.h>
#include <sharaku/log.h>
#include <sharaku/debug.h>

// 数学定義
#ifndef M_PI
#define M_PI	(3.14159265358979323846264332795f)
#endif
// 数学定義
#ifndef M_PI_180
#define M_PI_180	(3.14159265358979323846264332795f / 180.0f)
#endif


NAMESPACE_SHARAKU_BEGIN

sd_motors::sd_motors(int32_t wheel_axle_length, int32_t wheel_length)
 : _pid_l_motor(1.05f, 0.01f, 0.0f), _pid_r_motor(1.05f, 0.01f, 0.0f)
{
	sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);

	_wheel_axle_length	= wheel_axle_length;
	_wheel_length		= wheel_length;
	_steer_sp		= 0;
	_speed_sp		= 0;
	_steer			= 0;
	_speed			= 0;
	_position		= 0;
	_auto_correction	= 0;
	_prev_sum		= 0;
	_prev_deltas[0]		= 0;
	_prev_deltas[1]		= 0;
	_prev_deltas[2]		= 0;

	// 左右モータ補正
	_pid_l_motor.clear();
	_pos_l_motor		= 0;
	_dps_l_motor		= 0;
	_pid_r_motor.clear();
	_pos_r_motor		= 0;
	_dps_r_motor		= 0;

	sharaku_prof_init(&_prof_interval, "sd_motors::interval");
	sharaku_prof_init(&_prof_pre_update_process, "sd_motors::pre_update::processing");
	sharaku_prof_init(&_prof_post_update_process, "sd_motors::post_update::processing");
	debug_code(sharaku_prof_regist(&_prof_interval));
	debug_code(sharaku_prof_regist(&_prof_pre_update_process));
	debug_code(sharaku_prof_regist(&_prof_post_update_process));
}

// ステアリング角度を元にした制御
int32_t
sd_motors::set_speed_sp(int32_t dps)
{
	_speed_sp = _speed = dps;
	_dps_l_motor	= (int32_t)(((float)dps) * _vL);
	_dps_r_motor	= (int32_t)(((float)dps) * _vR);
	return 0;
}


// ステアリング角度を元にした制御
int32_t
sd_motors::set_steer_sp(int32_t steer_deg)
{
	differential(steer_deg);
	_steer_sp = _steer = steer_deg;
	_dps_l_motor	= (int32_t)(((float)_speed_sp) * _vL);
	_dps_r_motor	= (int32_t)(((float)_speed_sp) * _vR);
	return 0;
}

void
sd_motors::differential(int32_t steering)
{
	// まっすぐであれば、計算する必要なし
	if (steering == 0) {
		_vL = 1.0f;
		_vR = 1.0f;
		return;
	}
	if (steering == -90) {
		_vL = 1.0f;
		_vR = -1.0f;
		return;
	}
	if (steering == 90) {
		_vL = -1.0f;
		_vR = 1.0f;
		return;
	}

	register float rho = 0.0f;
	register float tan_theta = tan((float)steering * M_PI_180);
	register float vL = 0.0f;
	register float vR = 0.0f;

	// -------------------------------------------------------------------
	// ρ   = 旋回半径
	// θ   = ステアリング角度 = ω
	// ν   = 1回転の長さ = _wheel_length * M_PI
	// 上記の時、現在の旋回半径を求める
	//   ρ = v / tanθ
	// 計算式を成り立たせる
	rho = ((float)_wheel_length * M_PI) / tan_theta;
	_rho = rho;

	// -------------------------------------------------------------------
	// ρ   = 旋回半径	= rho
	// 2d   = 車輪間の長さ	= _wheel_axle_length
	// vL 	= 左タイヤの比率	= (ρ - d) ω
	// vR 	= 右タイヤの比率	= (ρ + d) ω
	// 
	// 3各関数を使用して比率を出す
	//   vL = (ρ - d) * tan θ
	//   vR = (ρ + d) * tan θ
	vL = (rho - _wheel_axle_length / 2) * tan_theta;
	vR = (rho + _wheel_axle_length / 2) * tan_theta;

	// -------------------------------------------------------------------
	// 各車輪間に比率補正をかける
	if(vL >= vR) {
		// -----------------------------------------------------------------
		// vLを100%とした際のvRを求める
		_vR = vR / vL;
		_vL = 1.0f;
	} else {
		// -----------------------------------------------------------------
		// vLを100%とした際のvRを求める
		_vL = vL / vR;
		_vR = 1.0f;
	}
	sharaku_db_trace("rho(*1000000)=%d vL(*1000000)=%d vR(*1000000)=%d "
			 "steering=%d",
			 (int32_t)(_rho * 1000000), (int32_t)(_vL * 1000000),
			 (int32_t)(_vR * 1000000), steering, 0, 0);
}

/******************************************************************************
IUpdate API
******************************************************************************/
int32_t
sd_motors::pre_update(const float &interval, uint32_t retry_cnt)
{
	sharaku_db_trace("interval=%d", (int32_t)(interval * 1000.0f), 0, 0, 0, 0, 0);

	// 両輪の状態を取得し、位置と速度を算出する
	register int32_t	left_pos	= out_speed_motor_l->get_position();
	register int32_t	right_pos	= out_speed_motor_r->get_position();

	register int32_t	pos_sum	= right_pos + left_pos;
	register int32_t	delta	= pos_sum - _prev_sum;
	_position	+= delta;
	_speed	= (float)((delta +
			_prev_deltas[0] +
			_prev_deltas[1] +
			_prev_deltas[2]) / (4.0f * interval));
	sharaku_db_trace("left_pos=%d right_pos=%d delta=%d _speed=%d _position=%d",
			 left_pos, right_pos, delta, _speed, _position, 0);

	// 情報を更新
	_prev_sum	= pos_sum;
	_prev_deltas[2] = _prev_deltas[1];
	_prev_deltas[1] = _prev_deltas[0];
	_prev_deltas[0] = delta;

	return 0;
}

int32_t
sd_motors::post_update(const float &interval, uint32_t retry_cnt)
{
	// 両輪の状態を取得し、位置と速度を算出する
	register int32_t	left_pos	= out_speed_motor_l->get_position();
	register int32_t	right_pos	= out_speed_motor_r->get_position();

	// 現在の位置との差分で速度を補正する。
	// 差分を速度に反映することでモータの個体差、電圧による速度差を吸収する。
	_pos_l_motor		+= _dps_l_motor * interval;
	_pos_r_motor		+= _dps_r_motor * interval;
	if (_auto_correction) {
		float power_left_per	= _pid_l_motor(interval, left_pos, _pos_l_motor);
		float power_right_per	= _pid_l_motor(interval, right_pos, _pos_r_motor);
		power_left_per	= power_left_per < 0 ? 0.0 : power_left_per > 200 ? 2.0 : power_left_per / 100.0f;
		power_right_per	= power_right_per < 0 ? 0.0 : power_right_per > 200 ? 2.0 : power_right_per / 100.0f;
		float power_left	= (float)_dps_l_motor * power_left_per;
		float power_right	= (float)_dps_r_motor * power_right_per;
		out_speed_motor_l->set_speed_sp(power_left);
		out_speed_motor_r->set_speed_sp(power_right);
		sharaku_db_trace("power_left=%d power_right=%d _pos_l_motor=%d _pos_r_motor=%d left_pos=%d right_pos=%d",
				power_left_per * 100, power_right_per * 100,
				_pos_l_motor, _pos_r_motor,
				left_pos, right_pos);
	} else {
		out_speed_motor_l->set_speed_sp(_dps_l_motor);
		out_speed_motor_r->set_speed_sp(_dps_r_motor);
		sharaku_db_trace("_dps_l_motor=%d _dps_r_motor=%d left_pos=%d right_pos=%d",
				_dps_l_motor, _dps_r_motor,
				left_pos, right_pos, 0, 0);
	}
	return 0;
}

NAMESPACE_SHARAKU_END
