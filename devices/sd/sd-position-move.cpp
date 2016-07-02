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
#define M_PI_180	(M_PI / 180.0f)
#endif
#ifndef M_180_PI
#define M_180_PI	(180.0f / M_PI)
#endif
#define RAG2DEG(rag)	((rag) * M_180_PI)
#define DEG2RAG(deg)	((deg) * M_PI_180)


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
	_arrival		= 3;		// 3mm
	_max_speed		= 1000;
	_min_speed		= 80;
	_proximity_deg		= 40;		// 40度
	_arrival_deg		= 0;		// 0度
	_max_speed_deg		= 400;
	_min_speed_deg		= 80;
	_steering		= 0;
	_target_dist		= 0;
	_target_dist_deg	= 0;
	
	_Kt			= 1.0f;
	_KtD			= 0.0f;
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
sd_position_move::set_position_sp(position3& pos)
{
	// ベジェ曲線を使用して指定位置まで進む。
	_target_pos = pos;
	_old_diff[0] = 0xffffffff;
	_old_diff[1] = 0xffffffff;
	_old_diff[2] = 0xffffffff;
	_mode = MODE_TARGET_POSITION;
	_status = STATUS_MOVING;
	return 0;
}

void
sd_position_move::update_position_mode(const float &interval)
{
	// 角度補正
	// b = X2 - X1
	// a = Y2 - Y1
	// Θ1 = atan(b/a)
	// Θ2 = 現在の角度
	// Θt = Θ1x - Θ2
	rotation3	rot = in_odo->get_rotation();
	position3	pos = in_odo->get_position();
	register float	a = _target_pos.x - pos.x;
	register float	b = _target_pos.y - pos.y;
	int32_t	diff = (int)sqrt(a * a + b * b);
	register float	theta = RAG2DEG(acos(a / diff));
	register float	theta_t = theta - rot.z;

	register float	delta_v = _Kt * theta_t + _KtD * 1 / theta_t;

	// 2度以下の場合、補正する方が誤差を生むので思い切って補正しない。
	if (-2.0f < delta_v && delta_v < 2.0f) {
		delta_v = 0.0f;
	}
	if (delta_v > 90.0f) {
		delta_v = 90.0f;
	} else if (delta_v < -90.0f) {
		delta_v = -90.0f;
	}
	_steering = (int32_t)(delta_v);

	// 残りの距離から速度と状態をとる
#if 0
	if (_status == STATUS_PROXIMITY &&
	    _old_diff[0] < diff &&
	    _old_diff[1] < _old_diff[0] &&
	    _old_diff[2] < _old_diff[1]) {
#else 
	if (diff == _arrival) {
#endif
		// 目的地を通り越した
		_status = STATUS_PASSING;
#if 0
		// Autoなら即停止する
		if (_auto) {
			goto stop;
		}
#else
_mode = MODE_STOP;
out_move->set_speed_sp((int32_t)0);
out_move->set_steer_sp((int32_t)0);
#endif
	} else if (diff <= _arrival)  {
		// 目的地に到着
		_status = STATUS_ARRIVAL;
		// Autoならここで停止する
#if 0
		if (_auto) {
			goto stop;
		}
#else
_mode = MODE_STOP;
out_move->set_speed_sp((int32_t)0);
out_move->set_steer_sp((int32_t)0);
#endif
	} else if (diff < _proximity) {
		// 目的地に接近
		_status = STATUS_PROXIMITY;
		// Autoなら速度を緩める
#if 0
		if (_auto) {
			goto slow_down;
		}
#else
out_move->set_speed_sp((int32_t)100);
out_move->set_steer_sp((int32_t)_steering);
#endif
	} else {
		// 目的地へ移動中
		_status = STATUS_MOVING;
#if 0
		if (_auto) {
			goto moving;
		}
#else
	if (delta_v > 45.0f) {
		out_move->set_speed_sp((int32_t)100);
	} else {
		out_move->set_speed_sp((int32_t)400);
	}
	out_move->set_steer_sp((int32_t)_steering);
#endif
	}

//	out_move->set_steer_sp(delta_v);
	goto out;

stop:
	out_move->set_speed_sp((int32_t)0);
	out_move->set_steer_sp((int32_t)0);
	_mode = MODE_STOP;
	goto out;

slow_down:
	// 線形で速度ダウンする
	int32_t	speed;
	speed = _max_speed
		 - ((_max_speed - _min_speed) / _proximity * (_proximity - diff));
	out_move->set_speed_sp((int32_t)speed);
	out_move->set_steer_sp((int32_t)_steering);
	goto out;

moving:
	// 指定のMax速度で移動する
	out_move->set_speed_sp((int32_t)_max_speed);
	out_move->set_steer_sp((int32_t)_steering);

out:
	sharaku_db_trace("_target_pos.x=%d _target_pos.y=%d pos.x=%d  pos.y=%d  rot.z=%d",
			 _target_pos.x, _target_pos.y, pos.x, pos.y, rot.z, 0);
	sharaku_db_trace("diff=%d theta=%d theta_t=%d  delta_v=%d",
			 diff, theta, theta_t, delta_v, 0, 0);
	_old_diff[2] = _old_diff[1];
	_old_diff[1] = _old_diff[0];
	_old_diff[0] = diff;

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
