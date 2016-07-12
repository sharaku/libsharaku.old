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
#include <devices/sd/sd-wheel-odometry.hpp>

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

static sharaku_prof_t	__prof_odometory_interval;
static sharaku_prof_t	__prof_odometory_processing;
sd_wheel_odometry::sd_wheel_odometry(int32_t wheel_axle_length,int32_t wheel_length)
{
	sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);

	sharaku_prof_init(&__prof_odometory_interval, "sd_wheel_odometry::interval");
	sharaku_prof_init(&__prof_odometory_processing, "sd_wheel_odometry::processing");
	sharaku_prof_regist(&__prof_odometory_interval);
	sharaku_prof_regist(&__prof_odometory_processing);

	_wheel_axle_length	= wheel_axle_length;
	_wheel_length		= wheel_length;
	_prev_count_r		= 0;
	_prev_count_l		= 0;
	_time			= 0;
	_distance		= 0;
	_theta			= 0.0f;
	_pos(0, 0, 0);
	_rot(0, 0, 0);
}

void
sd_wheel_odometry::pre_update(const float & interval)
{
	sharaku_db_trace("interval=%d", (int32_t)(interval * 1000.0f), 0, 0, 0, 0, 0);

	sharaku_usec_t time = sharaku_get_usec();
	if (_time) {
		sharaku_prof_add(&__prof_odometory_interval,
				 _time, time);
	}
	_time = time;

	// 角度単位の距離 (直径×3.1415 / 360)
	register const float rpd = (_wheel_length * M_PI) / 360.0f;

	// カウントを取り出す
	int32_t	left_pos	= in_speed_motor_l->get_position();
	int32_t	right_pos	= in_speed_motor_r->get_position();
	sharaku_db_trace("left_pos=%d right_pos=%d",
			 left_pos, right_pos, 0, 0, 0, 0);

	// 短い時間（delta）の間に移動した距離Lは本来弧であるが、
	// deltaが小さいときは直線として扱っても問題ないほど差は小さい。
	// よって、delta_left, delta_right, delta_Lは単純な計算でよい
	register int32_t	deg_delta_left	= left_pos - _prev_count_l;
	register int32_t	deg_delta_right	= right_pos - _prev_count_r;
	register float		deg_delta_L;

	deg_delta_L	= (float)(deg_delta_left + deg_delta_right) / 2.0f * rpd;
	_prev_count_l	= left_pos;
	_prev_count_r	= right_pos;

	// 現在の角度delta_thetaを算出する
	register float	delta_theta;
	delta_theta	= (((float)(deg_delta_right - deg_delta_left)) * rpd)
						 / (float)_wheel_axle_length;

	// 座標を加算
	_pos.x	= _pos.x + deg_delta_L * cos(DEG2RAG(_rot.z) + delta_theta);
	_pos.y	= _pos.y + deg_delta_L * sin(DEG2RAG(_rot.z) + delta_theta);

	// 角度を更新
	_theta += delta_theta;
	_rot.z = RAG2DEG(_theta);

	// ジャイロから角度を取得する
	if (in_gyro_x) {
		_rot.x = in_gyro_x->get_angle();
	}
	if (in_gyro_y) {
		_rot.y = in_gyro_y->get_angle();
	}

	_distance += deg_delta_L;

	sharaku_db_trace("x=%d y=%d distance=%d r.y=%d _theta=%d r.z=%d",
				(int32_t)_pos.x, (int32_t)_pos.y,
				(int32_t)_distance,
				(int32_t)_rot.y,
				(int32_t)(_theta * 180.0f / M_PI),
				(int32_t)_rot.z);

	// 時間収集
	time = sharaku_get_usec();
	sharaku_prof_add(&__prof_odometory_processing, _time, time);
	sharaku_db_trace("time=%d", (int32_t)(time - _time), 0, 0, 0, 0, 0);
}

NAMESPACE_SHARAKU_END
