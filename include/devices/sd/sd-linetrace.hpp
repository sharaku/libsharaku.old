/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2015
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_UV_MODULES_LINETRACE_H_
#define SHARAKU_UV_MODULES_LINETRACE_H_

#include <stdint.h>
#include <sharaku/utime.h>
#include <devices/update-api.hpp>
#include <devices/moving-api.hpp>
#include <devices/sensor-api.hpp>
#include <devices/connection_interface.hpp>

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------
class sd_linetrace
 : public move_operations
 , public update_operations
{
 public:
	enum edge {
		LINETRACE_EDGE_LEFT,
		LINETRACE_EDGE_RIGHT,
	};
	enum turn {
		LINETRACE_TURN_RIGHT,
		LINETRACE_TURN_LEFT,
	};
	enum target_line {
		LINETRACE_TARGET_IN_LINE,	// 目標の色(通常黒)を目標にする
		LINETRACE_TARGET_OUT_LINE,	// 目標外の色(通常白)を目標にする
	};

 public:
	sd_linetrace();

	// インターフェース接続
	connection_interface<move_operations>	out_move;
	connection_interface<light_operations>	in_photo;

	operator move_operations*() { return (move_operations*)this;}
	operator update_operations*() { return (update_operations*)this;}

 public:
	// traceを開始する
	virtual void trace_on(void) {
		_now_edge = _target_edge;
		_trace_onoff = 1;
	}
	// traceを一時停止する
	virtual void trace_off(void) {
		_now_edge = _target_edge;
		_trace_onoff = 0;
	}
	virtual void set_edge(edge e) {
		_target_edge = e;
	};
	// 白と判断する色を設定する
	virtual void set_white(uint32_t w) {
		_color_white = w;
	}
	// 黒と判断する色を設定する
	virtual void set_black(uint32_t b) {
		_color_black = b;
	}
	// 最大旋回角度を設定する
	//  in	黒に向かう方向の旋回角度
	//  out	白に向かう方向の旋回角度
	virtual void set_turn_angle(int32_t in, int32_t out) {
		_tangle_in = in;
		_tangle_out = out;
	}
	// trace状態を取得する
	virtual bool is_trace_on(void) {
		return _trace_onoff;
	}
	// 現在の検索方向(IN/OUT)を取得する
	virtual target_line get_target_line(void) {
		return _target_line;
	};
	// エッジのターゲットを取得する
	virtual edge get_target_edge(void) {
		return _target_edge;
	};
	// 現在のエッジを取得する
	virtual edge get_now_edge(void) {
		return _now_edge;
	};
	// ターゲットエッジの未検出時間を取得する
	virtual int32_t	get_uncertain_time(void) {
		return _uncertain_time;
	};
	// 白と判断する色を取得する
	virtual int32_t	get_color_white(void) {
		return _color_white;
	};
	// 黒と判断する色を取得する
	virtual int32_t	get_color_black(void) {
		return _color_black;
	};
	// 黒に向かう方向の旋回角度を取得する
	virtual int32_t	get_turn_angle_in(void) {
		return _tangle_in;
	};
	// 白に向かう方向の旋回角度を取得する
	virtual int32_t	get_turn_angle_out(void) {
		return _tangle_out;
	};

	// sm_move_op インタフェース
	virtual int32_t	get_steer_sp(void) {
		return out_move->get_steer_sp();
	}
	virtual int32_t	get_speed_sp(void) {
		return out_move->get_steer_sp();
	}
	virtual int32_t	set_speed_sp(int32_t dps) {
		_dps		= dps;
		if (!_trace_onoff) {
			out_move->set_speed_sp(_dps);
		}
		return 0;
	}
	virtual int32_t	set_steer_sp(int32_t steer) {
		_steering	= steer;
		if (!_trace_onoff) {
			out_move->set_steer_sp(_steering);
		}
		return 0;
	}
	virtual int32_t	get_speed(void) {
		return out_move->get_speed();
	}
	virtual int32_t	get_steer_deg(void) {
		return out_move->get_steer_deg();
	}
	virtual int32_t	get_position(void) {
		return out_move->get_position();
	}

	// update_operations インタフェース
	virtual void update(const float &interval);

 protected:
	// 黒であればtrueを白であればfalseを返す
	virtual bool color_check(void);
	// ターン方向が変わったときに呼ばれる
	virtual void reset_turn(void) {}
	// 補正をかけるために呼び出される
	virtual void traveling_correction(int turn, int32_t &power, int32_t &steering);
	// 黒と白の境目を取得する
	virtual int32_t	get_neutral_color(void) {
		// デフォルトでは、黒 ～ 白の中間点を白黒分岐点とする。
		return ((_color_black + _color_white) / 2) + _color_black;
	};
	// 光の差分を値を取得する
	virtual int32_t	get_diff_color(int32_t color) {
		// 中間値を0として-100 ～ +100で応答する。
		return color - get_neutral_color();
	};
	// 補正をかけた光値を0 ～ 100の間で取得する
	virtual int32_t	get_correction_color(int32_t color);

 private:
	sharaku_usec_t	_time;		// 前回実行時の時間

	uint32_t	_trace_onoff:1;
	float		_interval;

	// ターゲット色(黒)
	target_line	_target_line;
	// エッジ (設定値)
	edge		_target_edge;
	// 現在のエッジ
	edge		_now_edge;

	// 未発見時間
	int32_t		_uncertain_time;
	// ターゲット色(白)
	int32_t		_color_white;
	// ターゲット色(黒)
	int32_t		_color_black;
	// Speed
	int32_t		_dps;
	// Steering
	int32_t		_steering;
	// 旋回角度
	int32_t		_tangle_in;
	int32_t		_tangle_out;

 protected:
	virtual float	get_interval(void) {
		return _interval;
	};
	virtual int32_t	get_dps(void) {
		return _dps;
	};
	virtual int32_t	get_steering(void) {
		return _steering;
	};
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_UV_MODULES_LINETRACE_H_
