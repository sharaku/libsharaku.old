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
#include <devices/sd/sd-target-run.hpp>

// 数学定義
#ifndef M_PI
#define M_PI	( (float)3.14159265358979323846264332795 )
#endif
// 数学定義
#ifndef M_PI_180
#define M_PI_180	( M_PI / 180.0f)
#endif


NAMESPACE_SHARAKU_BEGIN

static sharaku_prof_t	__prof_target_run_interval;
static sharaku_prof_t	__prof_target_run_processing;

void
sd_target_run::update(const float &interval)
{
	sharaku_usec_t time = sharaku_get_usec();
	if (_time) {
		sharaku_prof_add(&__prof_target_run_interval,
				 _time, time);
	}
	_time = time;

	// 現在位置との差分を生成
	// 次の周期までに、指定位置へいる必要がある。
	// ただし、MAX速度的に不可能や、MAX旋回的に不可能である場合は除く。
	// 上記を実現するために以下の要領でspeed, steerを生成する。
	//  - 差分の座標(delta_pos)と周期(interval)からspeedを割り出す。
	//  - 差分の角度(delta_rot)と周期(interval)からsteerを割り出す。
	position3 delta_pos = _target_pos - in_odo->get_position();
	rotation3 delta_rot = _target_rot - in_odo->get_rotation();

	// 速度はdpsで算出する。このため、
	//   直線距離(mm) / 周期(interval)
	// で算出できる。delta_posは3次元座標である。
	// このため、 斜辺 ^ 2 = 底辺 ^ 2 + 高さ ^ 2 より、
	//   直線距離 = sqrt(dX ^ 2 + dY ^ 2)
	// で求められる。
	// 直線距離はmmで取得可能であるため、モーターの回転数(dps)は
	//   dps = 直線距離 / interval * 10000
	// で求められる。
	float delta_distance = sqrt(pow(delta_pos.x, 2) + pow(delta_pos.y, 2));
	float dps = delta_distance / interval * 1000000.0f;

	// 角度は相対角で算出する。このため、3次元角度のZ軸のみ使用する。
	// ただし、_min_turn ～ _max_turnの間で補正する。
	// 角度が _min_turn ～ _max_turnの範囲でない場合は、超過した分
	// 速度を反比例で落とす。
	float steer = delta_rot.z;
	if (steer < _min_turn) {
		steer = _min_turn;
		dps = dps * (1.0f / (delta_rot.z - _min_turn));
	} else if (steer > _max_turn) {
		steer = _max_turn;
		dps = dps * (1.0f / (delta_rot.z - _max_turn));
	}

	// moveへ設定
	out_move->set_speed_steer_sp((int32_t)dps, (int32_t)steer);

	// 時間収集
	time = sharaku_get_usec();
	sharaku_prof_add(&__prof_target_run_processing, _time, time);
}

NAMESPACE_SHARAKU_END
