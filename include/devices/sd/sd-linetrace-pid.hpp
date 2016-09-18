/*
 * Copyright Abe Takafumi All Rights Reserved, 2015-2016
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_UV_MODULES_LINETRACE_EXTENSION_H_
#define SHARAKU_UV_MODULES_LINETRACE_EXTENSION_H_

#include <stdint.h>
#include <devices/sd/sd-linetrace.hpp>
#include <sharaku/pid.hpp>
#include <sharaku/digital-filter.hpp>

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------
// PID制御を用いたライントレース
// 未実装
class sd_linetrace_pid
 : public sd_linetrace
{
 public:
	sd_linetrace_pid(float Kp, float Ki, float Kd)
	 : _pid(Kp, Ki, Kd), _lowpass(1.0f) {}
	virtual void set_trace_pid(float Kp, float Ki, float Kd) {
		_Kp = Kp;
		_Ki = Ki;
		_Kd = Kd;
	}
	virtual void set_lowpass(float q) {
		_lowpass_update_flag = 1;
		_q = q;
	}
	virtual void clear_pid(void) {
		_clear_flag = 1;
	}
	virtual void get_trace_pid(float &Kp, float &Ki, float &Kd) {
		Kp = _pid.get_Kp();
		Ki = _pid.get_Ki();
		Kd = _pid.get_Kd();
	}
	virtual float get_lowpass(void) {
		return _lowpass.get_q();
	}

 protected:
	virtual void on_black_event(void) {
		sd_linetrace::on_black_event();
		if (_lowpass_update_flag) {
			_lowpass.set(_q);
			_lowpass_update_flag = 0;
		}
		if (_pid_update_flag) {
			_pid.set_pid(_Kp, _Ki, _Kd);
			_pid_update_flag = 0;
		}
		if (_clear_flag) {
			_pid.clear();
			_clear_flag = 0;
		}
	}

 protected:
	virtual void traveling_correction(int turn,
					  int32_t &dps, int32_t &steering);

 private:
	sd_linetrace_pid() : _pid(0.0f, 0.0f, 0.0f), _lowpass(1.0f) {};
	low_pass_filter	_lowpass;
	pid		_pid;
	float 		_Kp, _Ki, _Kd, _q;
	int32_t		_clear_flag:1;
	int32_t		_lowpass_update_flag:1;
	int32_t		_pid_update_flag:1;
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_UV_MODULES_LINETRACE_EXTENSION_H_
