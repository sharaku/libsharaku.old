/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2015
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_DEVICE_SD_TARGET_RUN_H_
#define SHARAKU_DEVICE_SD_TARGET_RUN_H_

#include <stdint.h>
#include <sharaku/utime.h>
#include <sharaku/task.h>
#include <sharaku/pid.hpp>
#include <sharaku/position.hpp>
#include <devices/update-api.hpp>
#include <devices/moving-api.hpp>
#include <devices/odmetry-api.hpp>
#include <devices/connection_interface.hpp>

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------
// 車輪の回転数から座標を計算するクラス
class sd_position_move
 : public position_move_operations
 , protected move_operations
 , protected update_operations
{
 public:
	enum status {
		STATUS_STOP,
		STATUS_MOVING,
		STATUS_PROXIMITY,
		STATUS_ARRIVAL,
		STATUS_PASSING
	};
	enum mode {
		MODE_STOP,
		MODE_TARGET_DISTANCE,
		MODE_TARGET_DISTANCE_DEG_RIGHT,
		MODE_TARGET_DISTANCE_DEG_LEFT,
		MODE_TARGET_POSITION
	};

	// インターフェース接続
	connection_interface<move_operations>		out_move;
	connection_interface<odmetry_operations>	in_odo;

	operator position_move_operations*() { return (position_move_operations*)this;}
	operator update_operations*() { return (update_operations*)this;}

 public:
	sd_position_move(int32_t wheel_length);
	virtual ~sd_position_move() {}

	virtual mode get_mode(void) {
		return _mode;
	}
	virtual status get_status(void) {
		return _status;
	}

	// -------------------------------------------------------------
	// 自動Speed調整の設定
	// -------------------------------------------------------------
	virtual void move_on(void) {
		_mode = MODE_STOP;
		_move_onoff = 1;
	}
	virtual void move_off(void) {
		_move_onoff = 0;
	}
	virtual void set_autospeed_on(void) {
		_auto = 1;
	}
	virtual void set_autospeed_off(void) {
		_auto = 0;
	}
	virtual void set_limit_speed(int32_t min_speed, int32_t max_speed) {
		_speed_sp = max_speed;
		_max_speed = max_speed;
		_min_speed = min_speed;
		_max_speed_deg = max_speed;
		_min_speed_deg = min_speed;
	}
	virtual void set_proximity_arrival(int32_t proximity,
				      int32_t nearness,
				      int32_t arrival) {
		_proximity = proximity;
		_nearness = nearness;
		_arrival = arrival;
	}
	virtual void set_proximity_arrival_deg(int32_t proximity, int32_t arrival) {
		_proximity_deg = proximity;
		_arrival_deg = arrival;
	}
	virtual void set_pid(float Kp, float Ki, float Kd) {
		_pid.set_pid(Kp, Ki, Kd);
	}

	// -------------------------------------------------------------
	// move_operations
	// -------------------------------------------------------------
	virtual int32_t	get_steer_sp(void) {
		return _steer_sp;
	}
	virtual int32_t	get_speed_sp(void) {
		return _speed_sp;
	}
	virtual int32_t	get_speed(void) {
		return _speed;
	}
	virtual int32_t	set_steer_sp(int32_t steer_deg) {
		_steer_sp = steer_deg;
		return 0;
	}
	virtual int32_t	set_speed_sp(int32_t dps) {
		_speed_sp = dps;
		_max_speed = dps;
		return 0;
	}

	// -------------------------------------------------------------
	// 各動作は排他関係であり、後勝ちである。
	// -------------------------------------------------------------

	// 指定距離進んだら止まる
	// 距離は現在の距離からの相対位置
	virtual int32_t	set_trget_distance_sp(int32_t distance) {
		sharaku_db_trace("distance=%d", distance, 0, 0, 0, 0, 0);
		_mode = MODE_TARGET_DISTANCE;
		_status = STATUS_MOVING;
		_steer = 0;
		_target_dist = in_odo->get_dist() + distance;
		return _target_dist;
	}
	virtual int32_t	get_trget_distance_sp(void) {
		return _target_dist;
	}

	// 指定角度進んだら止まる
	// 角度は現在の角度からの相対角度
	virtual int32_t	set_trget_distance_deg_sp(int32_t rho, int32_t deg) {
		sharaku_db_trace("rho=%d deg=%d", rho, deg, 0, 0, 0, 0);
		_status = STATUS_MOVING;
		if (!deg) {
			return _steer;
		}
		
		if (deg < 0) {
			_steer = -1 * sharaku_rho2steering(rho, _wheel_length);
			_mode = MODE_TARGET_DISTANCE_DEG_RIGHT;
		} else {
			_steer = sharaku_rho2steering(rho, _wheel_length);
			_mode = MODE_TARGET_DISTANCE_DEG_LEFT;
		}

		const rotation3 rot = in_odo->get_rotation();
		_target_dist_deg = rot.z + deg;
		return _steer;
	}
	virtual int32_t	get_trget_distance_deg_sp(void) {
		return _steer;
	}

	// 指定位置指定まで進んだら止まる
	virtual int32_t	set_position_sp(position3& pos);
	virtual const position3& get_position_sp(void) {
		return _target_pos;
	}
	virtual int32_t	get_rest_distance(void) {
		return _rest_distance;
	}

	// -------------------------------------------------------------
	// update_operations インタフェース
	// -------------------------------------------------------------
	virtual void update(const float &interval);
	virtual void update_distance_mode(const float &interval);
	virtual void update_distance_deg_mode(const float &interval, int turn);
	virtual void update_position_mode(const float &interval);

 protected:
	// -------------------------------------------------------------
	// イベント通知
	// -------------------------------------------------------------
	virtual void _on_stop_event(void) {}
	virtual void _on_move_event(void) {}
	virtual void _on_proximity_event(void) {}
	virtual void _on_arrival_event(void) {}
	virtual void _on_passing_event(void) {}

 private:
	sd_position_move() : _pid(0.0f, 0.0f, 0.0f) {};
	virtual void _move_maxspeed(void) {
		_speed = _max_speed;
	}
	virtual void _move_slowdown(int32_t diff) {
		_speed = _max_speed
			 - ((_max_speed - _min_speed)
			 	 / _proximity * (_proximity - diff));
	}
	virtual void _move_maxspeed_deg(void) {
		_speed = _max_speed_deg;
	}
	virtual void _move_slowdown_deg(int32_t diff) {
		_speed = _max_speed_deg
			 - ((_max_speed_deg - _min_speed_deg)
			 	 / _proximity_deg * (_proximity_deg - diff));
	}
	virtual void _move_stop(void) {
		_speed = 0;
		_steer = 0;
		_mode = MODE_STOP;
		_on_stop_event();
	}

 private:
	sharaku_usec_t		_time;		// 前回実行時の時間
	uint32_t		_wheel_length;

	mode			_mode;		// モード
	status			_status;	// 状態

	// 設定
	uint32_t		_move_onoff:1;
	uint32_t		_auto:1;	// 速度の自動調整ON/OFF
	// 距離, 位置指定
	int32_t			_proximity;	// 接近しているとみなす距離
	int32_t			_nearness;	// 目的地間近とみなす距離
	int32_t			_arrival;	// 目的地とみなす距離
	int32_t			_max_speed;	// 自動最高速度
	int32_t			_min_speed;	// 自動最低速度
	int32_t			_target_dist;	// 指定距離進んだら止まるモード
						// での指定距離
	position3		_target_pos;
	// 角度指定
	int32_t			_proximity_deg;	// 接近しているとみなす角度
	int32_t			_arrival_deg;	// 目的地とみなす角度
	int32_t			_max_speed_deg;	// 自動最高速度
	int32_t			_min_speed_deg;	// 自動最低速度
	int32_t			_target_dist_deg; // 指定角度進んだら止まるモード
						  // での指定角度
	// 速度、角度設定
	int32_t			_steer_sp;	// 指定されたステアリング値
	int32_t			_speed_sp;	// 指定されたスピード値
	float			_steer; 	// 現在のステアリング値
	float			_speed; 	// 現在のスピード値

	float			_rest_distance; // 残り距離
	uint32_t		_old_diff[3];
	pid			_pid;
};

NAMESPACE_SHARAKU_END

#endif /* SHARAKU_DEVICE_SD_WHEELODO_H_ */
