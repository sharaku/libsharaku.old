/*
 * Copyright Abe Takafumi All Rights Reserved, 2015-2016
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_UV_PID_H_
#define SHARAKU_UV_PID_H_

#include <stdint.h>

//-----------------------------------------------------------------------------
// PID制御の実装
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
