/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2015
 * Author Abe Takafumi
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <devices/sd/sd-motors.hpp>
#include <sharaku/prof.h>
#include <sharaku/log.h>

// 数学定義
#ifndef M_PI
#define M_PI	(3.14159265358979323846264332795f)
#endif
// 数学定義
#ifndef M_PI_180
#define M_PI_180	(3.14159265358979323846264332795f / 180.0f)
#endif


NAMESPACE_SHARAKU_BEGIN

static sharaku_prof_t	__prof_motors_interval;
static sharaku_prof_t	__prof_motors_processing;

sd_motors::sd_motors(int32_t wheel_axle_length, int32_t wheel_length)
 : _pid_l_motor(1.05f, 0.01f, 0.0f), _pid_r_motor(1.05f, 0.01f, 0.0f)
{
	sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);

	sharaku_prof_init(&__prof_motors_interval, "sd_motors::interval");
	sharaku_prof_init(&__prof_motors_processing, "sd_motors::processing");
	sharaku_prof_regist(&__prof_motors_interval);
	sharaku_prof_regist(&__prof_motors_processing);

	_wheel_axle_length	= wheel_axle_length;
	_wheel_length		= wheel_length;
	_time			= 0;
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
void
sd_motors::pre_update(const float &interval)
{
	sharaku_db_trace("interval=%d", (int32_t)(interval * 1000.0f), 0, 0, 0, 0, 0);

	// 時間収集(最初の1回は採取しない)
	sharaku_usec_t time = sharaku_get_usec();
	if (_time) {
		sharaku_prof_add(&__prof_motors_interval,
				 _time, time);
	}
	_time = time;

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

	// 時間収集
	time = sharaku_get_usec();
	sharaku_prof_add(&__prof_motors_processing, _time, time);
	sharaku_db_trace("time=%d", (int32_t)(time - _time), 0, 0, 0, 0, 0);
}

void
sd_motors::post_update(const float &interval)
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

	if (out_device_update_l && out_device_update_r) {
		if(out_device_update_l->is_commited() &&
		   out_device_update_l->is_updated() &&
		   out_device_update_r->is_commited() &&
		   out_device_update_r->is_updated()) {
			// 両輪のupdate, commitが完了している場合のみ次をキックする。
			// そうしないと、片方のみ命令が発行され、ずれが生じる
			out_device_update_l->start_update();
			out_device_update_l->start_commit();
			out_device_update_r->start_update();
			out_device_update_r->start_commit();
		}
	}
}

NAMESPACE_SHARAKU_END
