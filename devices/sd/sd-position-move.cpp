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

sd_position_move::sd_position_move(int32_t wheel_length)
 : _pid(1.50f, 0.0f, 0.0f)
{
	sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);

	sharaku_prof_init(&__prof_position_move_interval, "sd_position_move::interval");
	sharaku_prof_init(&__prof_position_move_processing, "sd_position_move::processing");
	sharaku_prof_regist(&__prof_position_move_interval);
	sharaku_prof_regist(&__prof_position_move_processing);

	_time			= 0;
	_wheel_length		= wheel_length;

	_mode			= MODE_TARGET_DISTANCE;
	_status			= STATUS_STOP;

	_move_onoff		= 0;
	_auto			= 1;		// 自動速度調整ON

	_proximity		= 700;		// 700mm (70cm)
	_nearness		= 25;		// 25mm
	_arrival		= 3;		// 3mm
	_max_speed		= 1000;
	_min_speed		= 80;
	_target_dist		= 0;
	_target_pos(0.0f, 0.0f, 0.0f);

	_proximity_deg		= 40;		// 40度
	_arrival_deg		= 0;		// 0度
	_max_speed_deg		= 400;
	_min_speed_deg		= 80;
	_target_dist_deg	= 0;

	_steer_sp		= 0;
	_speed_sp		= 0;
	_steer			= 0;
	_speed			= 0;
	_old_diff[0] = 0xffffffff;
	_old_diff[1] = 0xffffffff;
	_old_diff[2] = 0xffffffff;
}

void
sd_position_move::update_distance_mode(const float &interval)
{
	// ODOから情報を取得し、現在位置と目的地の差分を出す。
	// 目的地に応じてステータスを変更
	int32_t	diff = _target_dist - in_odo->get_dist();
	sharaku_db_trace("diff=%d _arrival=%d _proximity=%d  _max_speed=%d  _min_speed=%d",
			 diff, _arrival, _proximity, _max_speed, _min_speed, 0);
	_rest_distance = diff;

	if (diff < 0) {
		// 目的地を通り越した
		_status = STATUS_PASSING;
		_on_passing_event();
		// Autoなら即停止する
		_move_stop();
	} else if (diff <= _arrival)  {
		// 目的地に到着
		_status = STATUS_ARRIVAL;
		_on_arrival_event();
		// Autoならここで停止する
		_move_stop();
	} else if (diff < _proximity) {
		// 目的地に接近
		_status = STATUS_PROXIMITY;
		_on_proximity_event();
		// Autoなら速度を緩める
		_move_slowdown(diff);
	} else {
		// 目的地へ移動中
		_status = STATUS_MOVING;
		_on_move_event();
		_move_maxspeed();
	}

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
			_on_passing_event();
			// Autoなら即停止する
			_move_stop();
		} else if (diff <= _arrival_deg)  {
			// 目的地に到着
			_status = STATUS_ARRIVAL;
			_on_arrival_event();
			// Autoならここで停止する
			_move_stop();
		} else if (diff < _proximity_deg) {
			// 目的地に接近
			_status = STATUS_PROXIMITY;
			_on_proximity_event();
			// Autoなら速度を線形で速度ダウンする
			_move_slowdown_deg(diff);
		} else {
			// 目的地へ移動中
			_status = STATUS_MOVING;
			_on_move_event();
			_move_maxspeed_deg();
		}
	} else {
		if (diff > 0) {
			// 目的地を通り越した
			_status = STATUS_PASSING;
			_on_passing_event();
			// Autoなら即停止する
			_move_stop();
		} else if (diff >= -_arrival_deg)  {
			// 目的地に到着
			_status = STATUS_ARRIVAL;
			_on_arrival_event();
			// Autoならここで停止する
			_move_stop();
		} else if (diff >= -_proximity_deg) {
			// 目的地に接近
			_status = STATUS_PROXIMITY;
			_on_proximity_event();
			// Autoなら速度を線形で速度ダウンする
			_move_slowdown_deg(-diff);
		} else {
			// 目的地へ移動中
			_status = STATUS_MOVING;
			_on_move_event();
			_move_maxspeed_deg();
		}
	}

	return;
}

int32_t
sd_position_move::set_position_sp(position3& pos)
{
	_target_pos = pos;
	_old_diff[0] = 0xffffffff;
	_old_diff[1] = 0xffffffff;
	_old_diff[2] = 0xffffffff;
	_rest_distance = 0x7fffffff;
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
	_rest_distance = diff;

	// PIDを使用して補正を行う
	_steer = _pid(interval, rot.z, theta);
	_steer = (_steer < -90) ? -90 : _steer;
	_steer = (_steer > 90) ? 90 : _steer;

	// 2度以下の場合、補正する方が誤差を生むので思い切って補正しない。
	if (-2 < _steer && _steer < 2) {
		_steer = 0;
	}

	// 残りの距離から速度と状態をとる
	if (_status == STATUS_PROXIMITY &&
	    _old_diff[0] < diff &&
	    _old_diff[1] < _old_diff[0] &&
	    _old_diff[2] < _old_diff[1]) {
		// 目的地を通り越した
		_status = STATUS_PASSING;
		_on_passing_event();
		// Autoなら即停止する
		_move_stop();
	} else if (diff <= _nearness &&
		   _old_diff[0] < diff &&
		   _old_diff[1] < _old_diff[0]) {
		// 目的地付近で連続的に遠ざかった場合、目的地に到着とみなす。
		_status = STATUS_ARRIVAL;
		_on_arrival_event();
		// Autoなら即停止する
		_move_stop();
	} else if (diff <= _nearness &&
		   _steer > 80.0f)  {
		// 目的地付近で旋回角が85以上の場合、目的地に到着とみなす。
		_status = STATUS_ARRIVAL;
		_on_arrival_event();
		// Autoなら微速前進
		_move_stop();
	} else if (diff <= _arrival)  {
		// 目的地との距離が一定値いないの場合は、到着とみなす。
		_status = STATUS_ARRIVAL;
		_on_arrival_event();
		// Autoなら微速前進
		_move_stop();
	} else if (diff < _proximity) {
		// 目的地に接近
		_status = STATUS_PROXIMITY;
		_on_proximity_event();
		// Autoなら速度を線形で速度ダウンする
		_move_slowdown(diff);
	} else {
		// 目的地へ移動中
		_status = STATUS_MOVING;
		_on_move_event();
		// 指定のMax速度で移動する
		_move_maxspeed();
	}

	sharaku_db_trace("_target_pos.x=%d _target_pos.y=%d pos.x=%d  pos.y=%d  rot.z=%d",
			 _target_pos.x, _target_pos.y, pos.x, pos.y, rot.z, 0);
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
//	if (_time) {
//		sharaku_prof_add(&__prof_motors_interval,
//				 _time, time);
//	}
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
	} else {
		_steer = _steer_sp;
		_speed = _speed_sp;
	}

	// AUTO速度調整がOFFであれば速度は指定されたものにする。
	if (!_auto) {
		_speed = _speed_sp;
	}

	sharaku_db_trace("_speed=%d _steer=%d mode=%d mode=%d _auto=%d",
			 _speed, _steer, _move_onoff, _auto, 0, 0);
	out_move->set_speed_sp((int32_t)_speed);
	out_move->set_steer_sp((int32_t)_steer);

	// 時間収集
	time = sharaku_get_usec();
//	sharaku_prof_add(&__prof_odometory_processing, _time, time);
	sharaku_db_trace("time=%d", (int32_t)(time - _time), 0, 0, 0, 0, 0);
}

NAMESPACE_SHARAKU_END
