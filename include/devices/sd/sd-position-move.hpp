/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2015
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_DEVICE_SD_TARGET_RUN_H_
#define SHARAKU_DEVICE_SD_TARGET_RUN_H_

#include <stdint.h>
#include <devices/api.hpp>
#include <sharaku/task.h>
#include <sharaku/position.hpp>

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------
// 車輪の回転数から座標を計算するクラス
class sd_position_move
 : public position_move_operations
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
	virtual status get_status(int32_t ms) {
		return _status;
	}

	// -------------------------------------------------------------
	// 自動Speed調整の設定
	// Param:	Kp, Ki, Kd	指定位置に対しての補正値
	//				速度に対する最大旋回角度の関係
	//				加速度
	//				減速度
	// -------------------------------------------------------------
	void	move_on(void) {
		_move_onoff = 1;
	}
	void	move_off(void) {
		_move_onoff = 0;
	}
	void	set_autospeed_on(void) {
		_auto = true;
	}
	void	set_autospeed_off(void) {
		_auto = false;
	}
	int32_t	set_max_speed(int32_t max_speed) {
		_max_speed = max_speed;
		return _max_speed;
	}
	int32_t	set_min_speed(int32_t min_speed) {
		_min_speed = min_speed;
		return _min_speed;
	}
	int32_t	get_proximity(int32_t proximity) {
		_proximity = proximity;
		return _proximity;
	}
	int32_t	get_arrival(int32_t arrival) {
		_arrival = arrival;
		return _arrival;
	}
	int32_t	get_proximity_deg(int32_t proximity) {
		_proximity_deg = proximity;
		return _proximity_deg;
	}
	int32_t	get_arrival_deg(int32_t arrival) {
		_arrival_deg = arrival;
		return _arrival_deg;
	}

	// -------------------------------------------------------------
	// 各動作は排他関係であり、後勝ちである。
	// -------------------------------------------------------------

	// 指定距離進んだら止まる
	int32_t	set_trget_distance_sp(int32_t distance) {
		sharaku_db_trace("distance=%d", distance, 0, 0, 0, 0, 0);
		_mode = MODE_TARGET_DISTANCE;
		_steering = 0;
		_target_dist = in_odo->get_dist() + distance;
		return _target_dist;
	}
	int32_t	get_trget_distance_sp(void) {
		return _target_dist;
	}

	// 指定角度進んだら止まる
	// rho(半径)をsteeringへ変換。degを距離にする。
	int32_t	set_trget_distance_deg_sp(int32_t rho, int32_t deg) {
		sharaku_db_trace("rho=%d deg=%d", rho, deg, 0, 0, 0, 0);
		if (!deg) {
			return _steering;
		}
		
		if (deg < 0) {
			_steering = -1 * sharaku_rho2steering(rho, _wheel_length);
			_mode = MODE_TARGET_DISTANCE_DEG_RIGHT;
		} else {
			_steering = sharaku_rho2steering(rho, _wheel_length);
			_mode = MODE_TARGET_DISTANCE_DEG_LEFT;
		}

		_target_dist_deg += deg;
		return _steering;
	}
	int32_t	get_trget_distance_deg_sp(void) {
		return _steering;
	}
	// 指定位置指定まで進んだら止まる
	int32_t	set_position_sp(position3& pos, rotation3& rot);
	const position3& get_position_sp(void) {
		return _target_pos;
	}

	const rotation3& get_rotation_sp(void) {
		return _target_rot;
	}
	// update_operations インタフェース
	virtual void update(const float &interval);
	virtual void update_distance_mode(const float &interval);
	virtual void update_distance_deg_mode(const float &interval, int turn);
	virtual void update_position_mode(const float &interval);

 private:
	sd_position_move() {};

 private:
	sharaku_usec_t		_time;		// 前回実行時の時間
	uint32_t		_wheel_length;

	mode			_mode;		// モード
	status			_status;	// 状態
	uint32_t		_move_onoff:1;
	bool			_auto;		// 速度の自動調整ON/OFF
	int32_t			_proximity;	// 接近しているとみなす距離
	int32_t			_arrival;	// 目的地とみなす距離
	int32_t			_proximity_deg;	// 接近しているとみなす角度
	int32_t			_arrival_deg;	// 目的地とみなす角度

	int32_t			_target_dist;	// 指定距離進んだら止まるモード
						// での指定距離
	int32_t			_target_dist_deg; // 指定角度進んだら止まるモード
						// での指定角度
	int32_t			_steering; 	// ステアリング値
	int32_t			_max_speed;	// 自動最高速度
	int32_t			_min_speed;	// 自動最低速度
	int32_t			_max_speed_deg;	// 自動最高速度
	int32_t			_min_speed_deg;	// 自動最低速度
	position3		_target_pos;
	rotation3		_target_rot;
};

NAMESPACE_SHARAKU_END

#endif /* SHARAKU_DEVICE_SD_WHEELODO_H_ */
