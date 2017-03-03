/* --
 *
 * MIT License
 * 
 * Copyright (c) 2015-2016 Abe Takafumi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <stdio.h>
#include <string.h>
#include <sharaku/log.h>
#include <devices/sd/sd-linetrace-pid.hpp>

NAMESPACE_SHARAKU_BEGIN

// 単純なPIDによる制御
void sd_linetrace_pid::traveling_correction(int turn, int32_t &dps, int32_t &steering)
{
	int32_t		steer_percent	= 0;
	int32_t		add_steer	= 0;
	int32_t		light_value	= 0;

	// カラー値取得と、-100% ～ 100%の値への変換を行う。
	light_value = get_correction_color(in_photo->get_value());

	// Low-Passフィルタを通す。
	light_value = _lowpass + light_value;

	// PIDを使用して旋回係数を求める。
	// 使用するインターバルは今回のインターバル値
	// ターゲットは-100 ～ 100の値に補正したカラーセンサー値
	// 目標は ±0
	// 出力範囲は-100% ～ 100%
	steer_percent = _pid(get_interval(), light_value, 0);
	steer_percent = (steer_percent < -100) ? -100 : steer_percent;
	steer_percent = (steer_percent > 100) ? 100 : steer_percent;

	// steering値に変換する
	if (steer_percent > 0) {
		add_steer = (steer_percent * get_turn_angle_out()) / 100;
	} else {
		add_steer = (steer_percent * get_turn_angle_in()) / 100;
	}

	dps		= get_dps();
	if (get_now_edge() == LINETRACE_EDGE_LEFT) {
		steering = get_steering() + add_steer;
	} else {
		steering = get_steering() - add_steer;
	}
}

NAMESPACE_SHARAKU_END

