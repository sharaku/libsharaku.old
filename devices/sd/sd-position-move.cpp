/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2015
 * Author Abe Takafumi
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sharaku/log.h>
#include <devices/sd/sd-position-move.hpp>

// 数学定義
#ifndef M_PI
#define M_PI	( (float)3.14159265358979323846264332795 )
#endif
// 数学定義
#ifndef M_PI_180
#define M_PI_180	( M_PI / 180.0f)
#endif


NAMESPACE_SHARAKU_BEGIN

static sharaku_prof_t	__prof_position_move_interval;
static sharaku_prof_t	__prof_position_move_processing;
sd_position_move::sd_position_move(int32_t wheel_length) {
	_time			= 0;
	_wheel_length		= wheel_length;
	_mode			= MODE_TARGET_DISTANCE;
	_status			= STATUS_STOP;
	_auto			= true;		// 自動速度調整ON
	_proximity		= 700;		// 700mm (70cm)
	_arrival		= 3;		// 5mm
	_max_speed		= 1020;
	_min_speed		= 80;
	_proximity_deg		= 40;		// 40度
	_arrival_deg		= 0;		// 0度
	_max_speed_deg		= 400;
	_min_speed_deg		= 50;
	_steering		= 0;
	_target_dist		= 0;
	_target_dist_deg	= 0;
}

void
sd_position_move::update_distance_mode(const float &interval)
{
	// ODOから情報を取得し、現在位置と目的地の差分を出す。
	// 目的地に応じてステータスを変更
	int32_t	diff = _target_dist - in_odo->get_dist();
	sharaku_db_trace("diff=%d _arrival=%d _proximity=%d  _max_speed=%d  _min_speed=%d",
			 diff, _arrival, _proximity, _max_speed, _min_speed, 0);

	if (diff < 0) {
		// 目的地を通り越した
		_status = STATUS_PASSING;
		// Autoなら即停止する
		if (_auto) {
			goto stop;
		}
	} else if (diff <= _arrival)  {
		// 目的地に到着
		_status = STATUS_ARRIVAL;
		// Autoならここで停止する
		if (_auto) {
			goto stop;
		}
	} else if (diff < _proximity) {
		// 目的地に接近
		_status = STATUS_PROXIMITY;
		// Autoなら速度を緩める
		if (_auto) {
			goto slow_down;
		}
	} else {
		// 目的地へ移動中
		_status = STATUS_MOVING;
		if (_auto) {
			goto moving;
		}
	}

 stop:
	out_move->set_speed_sp((int32_t)0);
	out_move->set_steer_sp((int32_t)0);
	return;

 moving:
	// 指定のMax速度で移動する
	out_move->set_speed_sp((int32_t)_max_speed);
	out_move->set_steer_sp((int32_t)_steering);
	return;

 slow_down:
	// 線形で速度ダウンする
	int32_t	speed;
	speed = _max_speed
		 - ((_max_speed - _min_speed) / _proximity * (_proximity - diff));
	out_move->set_speed_sp((int32_t)speed);
	out_move->set_steer_sp((int32_t)_steering);
	return;
}

void
sd_position_move::update_distance_deg_mode(const float &interval, int turn)
{
	// ODOから情報を取得し、現在位置と目的地の差分を出す。
	// 目的地に応じてステータスを変更
	int32_t	speed;
	rotation3 rot;
	rot = in_odo->get_rotation();
	int32_t	diff = _target_dist_deg - rot.z;

	sharaku_db_trace("diff=%d _arrival=%d _proximity=%d  _max_speed_deg=%d  _min_speed_deg=%d turn=%d",
			 diff, _arrival_deg, _proximity_deg, _max_speed_deg, _min_speed_deg, turn);

	if (turn > 0) {
		if (diff < 0) {
			// 目的地を通り越した
			_status = STATUS_PASSING;
			// Autoなら即停止する
			if (_auto) {
				goto stop;
			}
		} else if (diff <= _arrival_deg)  {
			// 目的地に到着
			_status = STATUS_ARRIVAL;
			// Autoならここで停止する
			if (_auto) {
				goto stop;
			}
		} else if (diff < _proximity_deg) {
			// 目的地に接近
			_status = STATUS_PROXIMITY;
			// Autoなら速度を緩める
			if (_auto) {
				goto slow_down;
			}
		} else {
			// 目的地へ移動中
			_status = STATUS_MOVING;
			if (_auto) {
				goto moving;
			}
		}
	} else {
		if (diff > 0) {
			// 目的地を通り越した
			_status = STATUS_PASSING;
			// Autoなら即停止する
			if (_auto) {
				goto stop;
			}
		} else if (diff >= -_arrival_deg)  {
			// 目的地に到着
			_status = STATUS_ARRIVAL;
			// Autoならここで停止する
			if (_auto) {
				goto stop;
			}
		} else if (diff >= -_proximity_deg) {
			// 目的地に接近
			_status = STATUS_PROXIMITY;
			// Autoなら速度を緩める
			if (_auto) {
				diff = -diff;
				goto slow_down;
			}
		} else {
			// 目的地へ移動中
			_status = STATUS_MOVING;
			if (_auto) {
				goto moving;
			}
		}
	}

 stop:
	// 停止する
	out_move->set_speed_sp((int32_t)0);
	out_move->set_steer_sp((int32_t)0);
	return;

 moving:
	// 指定のMax速度で移動する
	out_move->set_speed_sp((int32_t)_max_speed_deg);
	out_move->set_steer_sp((int32_t)_steering);
	return;

 slow_down:
	// 線形で速度ダウンする
	speed = _max_speed_deg
		 - ((_max_speed_deg - _min_speed_deg)
		 	 / _proximity_deg * (_proximity_deg - diff));
	out_move->set_speed_sp((int32_t)speed);
	out_move->set_steer_sp((int32_t)_steering);
	return;
}

int32_t
sd_position_move::set_position_sp(position3& pos, rotation3& rot)
{
#if 0
	// ベジェ曲線を使用して指定位置まで進む。
	// それに先立ち、ベジェ曲線の係数tを計算する。
	// tは 0 ～ 1 の値であり、
	//	係数 = 1/ (ポイント間の距離)
	// となる。
	// ポイント間の距離とは直線距離ではなく、ベジェ曲線の距離である。
	// これを出すために、仮に0.1刻みでポイント計算をして距離を出す。
	registor float	distance;
	position3	t_pos;
	_bezie.set_start_pos(pos);
	_bezie.set_end_pos(pos);

	float x1, x2, y1, y2;
	x1 = _bezie.get_start_pos().x;
	y1 = _bezie.get_start_pos().y;
	for (float t = 0.1f; t <= 1.0f; t += 0.1f) {
		// ベジェ曲線の位置を計算
		t_pos = _bezie.get_position(t);
		x2 = t_pos.x;
		y2 = t_pos.y;
		// 距離を加算する。
		// 位置から距離への変換は、
		//	sqrt((x1 - x2)^2 + (y1 - y2)^2)
		// で計算できる。
		distance += sqrt(powf(x1 - x2, 2.0f) + powf(y1 - y2, 2.0f));

		// 次のループで差分距離を出すために、END位置をSTART位置とする。
		x1 = x2;
		y1 = y2;
	}
	_target_dist	= distance;
	_Kt		= 0;
#endif
	// ベジェ曲線を使用して指定位置まで進む。
	_mode = MODE_TARGET_POSITION;
	return 0;
}

void
sd_position_move::update_position_mode(const float &interval)
{
#if 0
	int32_t	speed;
	int32_t	steering;
	int32_t	diff;

	// 残りの距離から速度と状態をとる
	if (diff < 0) {
		// 目的地を通り越した
		_status = STATUS_PASSING;
		// Autoなら即停止する
		if (_auto) {
			goto stop;
		}
	} else if (diff <= _arrival)  {
		// 目的地に到着
		_status = STATUS_ARRIVAL;
		// Autoならここで停止する
		if (_auto) {
			goto stop;
		}
	} else if (diff < _proximity) {
		// 目的地に接近
		_status = STATUS_PROXIMITY;
		// Autoなら速度を緩める
		if (_auto) {
			goto slow_down;
		}
	} else {
		// 目的地へ移動中
		_status = STATUS_MOVING;
		if (_auto) {
			goto moving;
		}
	}

 stop:
	// 停止する
	speed = 0;
	steering = 0;
	goto setting;

 moving:
	// 指定のMax速度で移動する
	speed = _max_speed;
	goto steering_setting;

 slow_down:
	// 線形で速度ダウンする
	speed = _max_speed
		 - ((_max_speed - _min_speed)
		 	 / _proximity * (_proximity - diff));
	goto steering_setting;

 steering_setting:
	// steeringはベジェ曲線を使用して移動させる
	// tの増分は、距離によって変化するので、あらかじめ計算しておく。
	//	tの増分 = (deg/sec * interval(s)) / 距離
	// で計算できる。
	// ベジェ曲線のtで計算した位置と現在の位置の差分から角度差分を決定し、。
	// 旋回角度を算出する。
	//	角度差分 = 
	registor float add_t = ((float)speed * interval) / _target_dist;
	_Kt += add_t;
	position3	t_pos;
	t_pos = _bezie.get_position(_Kt);
	_old_pos = t_pos;
	registor float rot = 0.0f;

	// 計算で求めた次のupdate時にいるべき位置へ移動できるように
	// steeringと設定する。
	steering = 0;

 setting:
	out_move->set_speed_sp(speed);
	out_move->set_steer_sp(steering);
#endif
	return;
}

void
sd_position_move::update(const float &interval)
{
	sharaku_db_trace("interval=%d", (int32_t)(interval * 1000.0f), 0, 0, 0, 0, 0);
	// 時間収集(最初の1回は採取しない)
	sharaku_usec_t time = sharaku_get_usec();
	if (_time) {
		sharaku_prof_add(&__prof_motors_interval,
				 _time, time);
	}
	_time = time;

	if (_move_onoff) {
		switch(_mode) {
		case MODE_TARGET_DISTANCE:
			update_distance_mode(interval);
			break;
		case MODE_TARGET_DISTANCE_DEG_RIGHT:
			update_distance_deg_mode(interval, -1);
			break;
		case MODE_TARGET_DISTANCE_DEG_LEFT:
			update_distance_deg_mode(interval, 1);
			break;
		case MODE_TARGET_POSITION:
			update_position_mode(interval);
			break;
		}
	}

	// 時間収集
	time = sharaku_get_usec();
//	sharaku_prof_add(&__prof_odometory_processing, _time, time);
	sharaku_db_trace("time=%d", (int32_t)(time - _time), 0, 0, 0, 0, 0);
}

NAMESPACE_SHARAKU_END
