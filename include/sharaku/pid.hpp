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
 * SOFTWARE. *
 *
 */

#ifndef SHARAKU_UV_PID_H_
#define SHARAKU_UV_PID_H_

#include <stdint.h>

//-----------------------------------------------------------------------------
// PID制御の実装
//  PID特性
//   Kp
//     targetとnowとの差分に比例する。
//     目標との差が大きいほど出力値も大きくなる。
//     目標値との差分がそのまま出力値に反映される。
//   Ki
//     targetとnowとの差分を積算した値の比例する。
//     前回の出力値が大きいほど出力値も大きくなる
//     出力値の変化が緩やかになる。
//   Kd
//    targetとnowとの差分と前回差分との差の大きさに比例する。
//    変化が大きいときに出力値も大きくなる。
//    瞬間的な変化が出力値に反映される。
class pid
{
 public:
	pid(float Kp, float Ki, float Kd) {
		set_pid(Kp, Ki, Kd);
		clear();
	}
	void clear(void) {
		_ei = 0.0f;
		_el = 0.0f;
	}
	void set_pid(float Kp, float Ki, float Kd) {
		_Kp = Kp;
		_Ki = Ki;
		_Kd = Kd;
	}
	float operator()(float delta_ms, int32_t now, int32_t target) {
		register float	u = 0.0f;
		register float	e = 0.0f;
		register float	ed = 0.0f;

		// Δ時間T
		// 誤差e      = 目標値 - 現在値
		// 誤差積分ei = ei + e * T
		// 誤差微分ed = (e - 前回誤差el) / T
		// 前回誤差el = e
		e	= target - now;			// 誤差
		_ei	= _ei + e * delta_ms;		// 誤差積分
		ed	= (e - _el) / delta_ms;		// 誤差微分
		_el = e;

		// PID計算
		// 操作量u = 比例ゲインKP * e
		//           + 積分ゲインKI * ei
		//           + 微分ゲインKD * ed
		u = _Kp * e + _Ki * _ei + _Kd * ed;

		return u;
	}
 public:
	float get_Kp(void) { return _Kp; }
	float get_Ki(void) { return _Ki; }
	float get_Kd(void) { return _Kd; }

	float get_ei(void) { return _ei; }
	float get_el(void) { return _el; }
 protected:
	float	_Kp;
	float	_Ki;
	float	_Kd;
	float	_ei;		// 誤差積分
	float	_el;		// 前回誤差

 private:
	pid() {}
};


#endif // SHARAKU_UV_PID_H_
