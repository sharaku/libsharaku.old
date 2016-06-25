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
		_pid.set_pid(Kp, Ki, Kd);
		_pid.clear();
	}
	virtual void SetLowpass(float q) {
		_lowpass.set(q);
	}
	virtual void clear_pid(void) {
		_pid.clear();
	}

 protected:
	virtual void traveling_correction(int turn,
					  int32_t &dps, int32_t &steering);

 private:
	sd_linetrace_pid() : _pid(0.0f, 0.0f, 0.0f), _lowpass(1.0f) {};
	low_pass_filter	_lowpass;
	pid		_pid;
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_UV_MODULES_LINETRACE_EXTENSION_H_
