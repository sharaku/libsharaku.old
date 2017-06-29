/* --
 *
 * MIT License
 * 
 * Copyright (c) 2017 Abe Takafumi
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

#include <stdio.h>
#include <string.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <devices/motor/simuration-motor.hpp>

NAMESPACE_SHARAKU_BEGIN
namespace simuration {

motor_simuration::motor_simuration(int rpm)
{
	set_rpm(rpm);
}

void
motor_simuration::set_rpm(int32_t rpm)
{
	int max_dps = (rpm / 60.0f) * 360.0f;
	int i;

	_position = 0;
	_duty_cycle = 0;
	_duty_cycle_sp = 0;

	// duty値による角度加算テーブル。
	// サンプルは線形。実際にこんなことはあり得ないが...
	// モータごとの特性もここで表現する。
	
	for (i = 0; i < 101; i++) {
		_duty2dps[i] = (max_dps / 100.0f) * i;
	}
}

void
motor_simuration::update(float second)
{
	int dps;

	_duty_cycle = _duty_cycle_sp;
	if (_duty_cycle > 0) {
		// 正回転の場合、テーブルに登録されている値を使用する
		dps = _duty2dps[_duty_cycle];
	} else if (_duty_cycle < 0) {
		// 逆回転の場合、テーブルに登録されている値にマイナス
		// を掛けたものを使用する。
		dps = -_duty2dps[-_duty_cycle];
	} else {
		// 0の場合は0
		dps = 0;
	}
	_position += dps * second;
}

tacho_motor_simuration::tacho_motor_simuration()
 : motor_simuration(0)
 , _hold_pid(1.0, 0.0f, 0.0f)
 , _speed_pid(1.0, 0.0f, 0.0f)
{
	_position_mode = 0;
	_position_sp = 0;

	_speed_mode = 0;
	_speed = 0;
	_speed_sp = 0;
}

tacho_motor_simuration::tacho_motor_simuration(int rpm)
 : motor_simuration(rpm)
 , _hold_pid(6.0, 0.0f, 0.0f)
 , _speed_pid(6.0, 0.0f, 0.0f)
{
	_position_mode = 0;
	_position_sp = 0.0f;

	_speed_mode = 0;
	_speed = 0;
	_speed_sp = 0;
}

void
tacho_motor_simuration::update(float second)
{
	if (_position_mode) {
#if 0
		printf("_position_mode=%d second=%d pos=%d target=%d pid=%f\n",
			_position_mode,
			(int32_t)(second * 1000),
			get_position(),
			(int32_t)_position_sp,
			_hold_pid(second * 1000, (int32_t)get_position(), (int32_t)_position_sp));
#endif
		// 今の角度とターゲットとの差異をpidで制御する
		set_duty_cycle_sp(_hold_pid(second * 1000, (int32_t)get_position(), (int32_t)_position_sp));
	}
	if (_speed_mode) {
		// degree per secに合うようにduty値を制御する
		_speed = _speed_sp;
		_targetpos = _targetpos + _speed * second;
#if 0
		printf("_position_mode=%d second=%d pos=%d target=%d pid=%f\n",
			_speed_mode,
			(int32_t)(second * 1000),
			get_position(),
			(int32_t)_targetpos,
			_speed_pid(second * 1000, (int32_t)get_position(), (int32_t)_targetpos));
#endif
		// 今の角度とターゲットとの差異をpidで制御する
		set_duty_cycle_sp(_speed_pid(second * 1000, (int32_t)get_position(), (int32_t)_targetpos));
	}

	motor_simuration::update(second);
}

};

NAMESPACE_SHARAKU_END
