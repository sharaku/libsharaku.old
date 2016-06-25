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
class sd_target_run
 : public target_pos_operations
 , protected update_operations
{
 public:
	// インターフェース接続
	connection_interface<odmetry_operations>	in_odo;
	connection_interface<move_operations>		out_move;

	operator target_pos_operations*() { return (target_pos_operations*)this;}
	operator update_operations*() { return (update_operations*)this;}

 public:
	sd_target_run() {
		_min_turn = -70.0f;
		_max_turn = 70.0f;
	}
	virtual ~sd_target_run() {}


	int32_t	set_position_sp(position3& pos) {
		_target_pos = pos;
		return 0;
	}
	const position3& get_position_sp(void) {
		return _target_pos;
	}
	int32_t	set_rotation_sp(rotation3& rot) {
		_target_rot = rot;
		return 0;
	}
	const rotation3& get_rotation_sp(void) {
		return _target_rot;
	}

	// sm_update_op インタフェース
	virtual void update(const float &interval);

 private:
	sharaku_usec_t		_time;			// 前回実行時の時間

	float			_min_turn;
	float			_max_turn;
	position3		_target_pos;
	rotation3		_target_rot;
};

NAMESPACE_SHARAKU_END

#endif /* SHARAKU_DEVICE_SD_WHEELODO_H_ */
