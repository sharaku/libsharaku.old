/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2015
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_DEVICE_SD_WHEELODO_H_
#define SHARAKU_DEVICE_SD_WHEELODO_H_

#include <stdint.h>
#include <devices/api.hpp>
#include <sharaku/task.h>
#include <sharaku/position.hpp>

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------
// 車輪の回転数から座標を計算するクラス
class sd_wheel_odometry
 : public odmetry_operations
 , protected update_operations
{
 public:
	// インターフェース接続
	connection_interface<speed_motor_operations>	in_speed_motor_l;
	connection_interface<speed_motor_operations>	in_speed_motor_r;
	connection_interface<gyro_operations>		in_gyro_x;	// X軸のGyro
	connection_interface<gyro_operations>		in_gyro_y;	// Y軸のGyro

	operator odmetry_operations*() { return (odmetry_operations*)this;}
	operator update_operations*() { return (update_operations*)this;}

 public:
	sd_wheel_odometry(int32_t wheel_axle_length, int32_t wheel_length);
	virtual ~sd_wheel_odometry() {}

	virtual const position3& get_position(void) {
		return _pos;
	}
	virtual int32_t set_position(const position3& pos) {
		_pos = pos;
		return 0;
	}
	virtual const rotation3& get_rotation(void) {
		return _rot;
	}
	virtual int32_t set_rotation(const rotation3& rot) {
		_rot = rot;
		return 0;
	}
	virtual const int32_t get_dist(void) {
		return (int32_t)_distance;
	}
	virtual int32_t set_dist(int32_t dist) {
		_distance = (float)dist;
		return 0;
	}
	virtual int32_t reset(void) {
 		// 単位はmmです。
		_prev_count_r	= 0;
		_prev_count_l	= 0;
		_pos(0, 0, 0);
		_rot(0, 0, 0);
		_distance	= 0;
		return 0;
	}

	// update_operations インタフェース
	virtual void pre_update(const float &interval);

 private:
	sd_wheel_odometry() {}

	// 設定値
	int32_t			_wheel_axle_length;	// 車輪間の距離
	int32_t			_wheel_length;		// 車輪の径

 private:
	sharaku_usec_t		_time;			// 前回実行時の時間

	int32_t			_prev_count_r;
	int32_t			_prev_count_l;
	position3		_pos;			// 累積位置
	rotation3		_rot;			// 累積角度
	float			_theta;
	float			_distance;
};

NAMESPACE_SHARAKU_END

#endif /* SHARAKU_DEVICE_SD_WHEELODO_H_ */
