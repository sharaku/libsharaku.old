/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

/******************************************************************************
include section
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sharaku/prof.h>
#include <sharaku/log.h>
#include <devices/sd/sd-linetrace.hpp>

#define PROFILE
//#define DEFAULT_LINETRACE

NAMESPACE_SHARAKU_BEGIN

/******************************************************************************
definition section
******************************************************************************/
static const int	_table_in[2] = {
	// エッジ(左)				エッジ(右)
	sd_linetrace::LINETRACE_TURN_RIGHT,	sd_linetrace::LINETRACE_TURN_LEFT
};
static const int	_table_out[2] = {
	// エッジ(左)				エッジ(右)
	sd_linetrace::LINETRACE_TURN_LEFT,	sd_linetrace::LINETRACE_TURN_RIGHT
};


/******************************************************************************
class section
******************************************************************************/

sd_linetrace::sd_linetrace()
{
	sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);

	_trace_onoff	= 0;
	_interval	= 0.0f;

	_target_line	= LINETRACE_TARGET_IN_LINE;
	_target_edge	= LINETRACE_EDGE_LEFT;
	_now_edge	= LINETRACE_EDGE_LEFT;

	_uncertain_time	= 0;
	_color_white	= 60;
	_color_black	= 0;
	_dps		= 0;
	_steering	= 0;
	_tangle_in	= 30;
	_tangle_out	= 30;

	sharaku_prof_init(&_prof_interval, "sd_linetrace::interval");
	sharaku_prof_init(&_prof_update_process, "sd_linetrace::processing");
	sharaku_prof_regist(&_prof_interval);
	sharaku_prof_regist(&_prof_update_process);
}


int32_t sd_linetrace::update(const float &interval, uint32_t retry_cnt)
{
	sharaku_db_trace("interval=%d", (int32_t)(interval * 1000.0f), 0, 0, 0, 0, 0);

	int32_t		dps = 0;
	int32_t		steering = 0;

	sharaku_db_trace("", 0, 0, 0, 0, 0, 0);

	if (!_trace_onoff) {
		return 0;
	}

	// 不確定時間を加算する
	_interval = interval;
	_uncertain_time += interval * 1000.0f;

	// 現在の検索方向を取得
	int old_turn = (_target_line = LINETRACE_TARGET_IN_LINE) ?
				_table_in[_now_edge] : _table_out[_now_edge];

	// 現在の光の値を評価する。エッジの更新もこのタイミングで行う。
	// エッジの更新は黒の時のみ行うそうでないと、黒の検索が破たんする。
	if (color_check()) {
		on_black_event();
	} else {
		on_white_event();
	}
	sharaku_db_trace("_target_line=%d _now_edge=%d",
			 _target_line, _now_edge, 0, 0, 0, 0);

	// 現在の探している色を元に旋回方向を出す。
	// 色とエッジの組み合わせを旋回方向へ変換する
	int turn = (_target_line = LINETRACE_TARGET_IN_LINE) ?
				_table_in[_now_edge] : _table_out[_now_edge];
	if (old_turn != turn) {
		reset_turn();
	}

	// 現在の旋回方向、色、時間、速度等を元にpower, steeringに補正をかける
	traveling_correction(turn, dps, steering);
	sharaku_db_trace("TravelingCorrection turn=%d, dps=%d, steering=%d",
			 turn, dps, steering, 0, 0, 0);

	// move_operationsに対して出力を行う
	out_move->set_speed_sp(dps);
	out_move->set_steer_sp(steering);

	return 0;
}


bool
sd_linetrace::color_check(void)
{
	// カラーセンサーの値を取る
	int32_t light	= in_photo->get_value();
	sharaku_db_trace("light=%d", light, 0, 0, 0, 0, 0);

	// 現在の色が中間色よりも低い値であれば線の上にいると判断する
	if (light < get_neutral_color()) {
		return true;
	} else {
		return false;
	}
}


void
sd_linetrace::on_black_event(void)
{
	// もし、エッジ変更が必要な場合はエッジ変更する
	if (_now_edge != _target_edge) {
		_now_edge	= _target_edge;
	}

	// ラインを目指しており、かつ現在ライン上である場合、
	// ターゲットをラインから外れるように設定する
	if (_target_line == LINETRACE_TARGET_IN_LINE) {
		_target_line	= LINETRACE_TARGET_OUT_LINE;
		_uncertain_time = 0;
	}
}


void
sd_linetrace::on_white_event(void)
{
	// ラインから外れるように動作しており、かつ白を検知した場合は
	// ラインを目指すように設定
	if (_target_line == LINETRACE_TARGET_OUT_LINE) {
		// 今非ラインを目指していた
		_target_line	= LINETRACE_TARGET_IN_LINE;
		_uncertain_time = 0;
	}
}


// 単純なライントレース
void
sd_linetrace::traveling_correction(int turn, int32_t &dps, int32_t &steering)
{
	if (get_target_line() == LINETRACE_TARGET_IN_LINE) {
		if (turn == LINETRACE_TURN_RIGHT) {
			// 右を目指している場合
			steering = _steering + get_turn_angle_in();
		} else {
			// 左を目指している場合
			steering = _steering - get_turn_angle_in();
		}
	} else {
		if (turn == LINETRACE_TURN_RIGHT) {
			// 右を目指している場合
			steering = _steering + get_turn_angle_out();
		} else {
			// 左を目指している場合
			steering = _steering - get_turn_angle_out();
		}
	}

	// 入力されたスピードを指定
	dps = _dps;
	sharaku_db_trace("dps=%d, steering=%d",
			 dps, steering, 0, 0, 0, 0);
}

// OUTPUT: -100 ～ 100
int32_t
sd_linetrace::get_correction_color(int32_t color) {
	int32_t	target_light	= get_neutral_color();
	int32_t	diff_light	= get_diff_color(color);
	int32_t	result = 0;

	if (diff_light < 0) {
		// 黒の範囲である。かつ、設定値よりも黒の値が小さい場合、
		// 計算上-100%以上の値が出るため、補正する
		if (diff_light < (get_color_black() - target_light)) {
			result = -100;
		} else {
			// (get_color_black() - target_light) ～ 0の値を
			// -100 ～ 0の値へ変換する
			result = (diff_light * 100)
					/ (target_light - get_color_black());
		}
	} else {
		// 白の範囲である。かつ、設定値よりも白の値が小さい場合、
		// 計算上100%以上の値が出るため、補正する
		if (diff_light < (get_color_white() - target_light)) {
			result = 100;
		} else {
			// 0 ～ (get_color_white() - target_light)の値を
			// 0 ～ +100の値へ変換する
			result = (diff_light * 100)
					 / (get_color_white() - target_light);
		}
	}

	return result;
};

NAMESPACE_SHARAKU_END

