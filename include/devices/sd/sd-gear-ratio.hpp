/* --
 *
 * MIT License
 * 
 * Copyright (c) 2014-2016 Abe Takafumi
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

#ifndef SHARAKU_UV_MODULES_GEAR_RATIO_H_
#define SHARAKU_UV_MODULES_GEAR_RATIO_H_

#include <stdint.h>
#include <devices/motor-api.hpp>

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------

// ギア比補正をかけるモジュールです
// RATIOには、ギア比を指定します。
// タイヤ側3、モータ側1の場合、OUT_GEAR = 3, IN_GEAR = 1を指定します。
// タイヤ側1、モータ側8の場合、OUT_GEAR = 1, IN_GEAR = 8を指定します。
// ギアが偶数の場合で符号を反転させたい場合、OUT_GEAR側に-の値を指定してください。
// モータの誤差補正を行う場合もGearRatioを使用する。

#define GEAR_RATIO(IN, OUT) ((float)OUT_GEAR / (float)IN_GEAR)

class sd_gearratio_speed
 : virtual public speed_motor_operations
{
 public:
	// インターフェース接続(複数connect禁止)
	connection_interface<speed_motor_operations>	out_motor;

	operator speed_motor_operations* () {
		return (speed_motor_operations*) this;
	}

 public:
	sd_gearratio_speed() {
		gear_ratio = 1.0f;
		pos_ratio = 1.0f;
	}

	void set_gear_ratio(float ratio) {
		gear_ratio = ratio;
		pos_ratio = ratio;
	}

	void set_gear_ratio(float gear, float pos) {
		gear_ratio = gear;
		pos_ratio = pos;
	}

	int32_t	get_speed(void) {
		int32_t	rpm;
		if ((speed_motor_operations*)out_motor) {
			rpm = out_motor->get_speed();
		} else {
			rpm = 0;
		}
		// ギヤ比補正をかける
		return (int32_t)((float)rpm * gear_ratio);
	}
	int32_t	set_speed_sp(int32_t rpm) {
		if ((speed_motor_operations*)out_motor) {
			int32_t ratio_speed = (float)rpm / gear_ratio;
			return out_motor->set_speed_sp(ratio_speed);
		} else {
			return 0;
		}
	}
	int32_t	get_speed_sp(void) {
		if ((speed_motor_operations*)out_motor) {
			return out_motor->get_speed_sp();
		} else {
			return 0;
		}
	}
	int32_t	get_count_per_rot(void) {
		if ((speed_motor_operations*)out_motor) {
			return out_motor->get_count_per_rot() / gear_ratio;
		} else {
			return 0;
		}
	}
	int32_t	get_position(void) {
		int32_t	pos = 0;
		if ((speed_motor_operations*)out_motor) {
			pos = out_motor->get_position();

		}
		// ギヤ比補正をかける
		return (int32_t)((float)pos * pos_ratio);
	}
	int32_t	set_position(int32_t pos) {
		int32_t	result = (int32_t)((float)pos * pos_ratio);
		// ギヤ比補正をかける
		if ((speed_motor_operations*)out_motor) {
			return out_motor->set_position(result);
		} else {
			return 0;
		}
	}
	int32_t get_stop_mode(void) {
		if ((speed_motor_operations*)out_motor) {
			return out_motor->get_stop_mode();
		} else {
			return 0;
		}
	}
	int32_t set_stop_mode(stop_mode mode) {
		if ((speed_motor_operations*)out_motor) {
			return out_motor->set_stop_mode(mode);
		} else {
			return 0;
		}
	}
	int32_t	set_speed_pid(int kp,int ki, int kd) {
		if ((speed_motor_operations*)out_motor) {
			return out_motor->set_speed_pid(kp, ki, kd);
		} else {
			return 0;
		}
	}
private:
	float gear_ratio;
	float pos_ratio;
};

class sd_gearratio_angle
 : virtual public angle_motor_operations
{
 public:
	// インターフェース接続(複数connect禁止)
	connection_interface<angle_motor_operations>	out_motor;

	operator angle_motor_operations* () {
		return (angle_motor_operations*) this;
	}

 public:
	sd_gearratio_angle() {
		gear_ratio = 1.0f;
		pos_ratio = 1.0f;
	}

	void set_gear_ratio(float ratio) {
		gear_ratio = ratio;
		pos_ratio = ratio;
	}

	void set_gear_ratio(float gear, float pos) {
		gear_ratio = gear;
		pos_ratio = pos;
	}

	int32_t	get_position_sp(void) {
		if ((angle_motor_operations*)out_motor) {
			return out_motor->get_position_sp();
		} else {
			return 0;
		}
	}
	int32_t	set_position_sp(int32_t pos) {
		if ((angle_motor_operations*)out_motor) {
			int32_t	ratio_pos = (int32_t)((float)pos / gear_ratio);
			return out_motor->set_position_sp(ratio_pos);
		} else {
			return 0;
		}
	}
	int32_t	add_position_sp(int32_t pos) {
		if ((angle_motor_operations*)out_motor) {
			int32_t	ratio_pos = (int32_t)((float)pos / gear_ratio);
			return out_motor->add_position_sp(ratio_pos);
		} else {
			return 0;
		}
	}

	int32_t	get_position(void) {
		int32_t	pos = 0;
		if ((angle_motor_operations*)out_motor) {
			pos = out_motor->get_position();
		}
		// ギヤ比補正をかける
		return (int32_t)((float)pos * pos_ratio);
	}
	int32_t	set_position(int32_t pos) {

		// ギヤ比補正をかける
		if ((angle_motor_operations*)out_motor) {
			int32_t	ratio_pos = (int32_t)((float)pos * pos_ratio);
			return out_motor->set_position(ratio_pos);
		} else {
			return 0;
		}
	}
	int32_t get_stop_mode(void) {
		if ((angle_motor_operations*)out_motor) {
			return out_motor->get_stop_mode();
		} else {
			return 0;
		}
	}
	int32_t set_stop_mode(stop_mode mode) {
		if ((angle_motor_operations*)out_motor) {
			return out_motor->set_stop_mode(mode);
		} else {
			return 0;
		}
	}
	int32_t	set_hold_pid(int kp,int ki, int kd) {
		if ((angle_motor_operations*)out_motor) {
			return out_motor->set_hold_pid(kp, ki, kd);
		} else {
			return 0;
		}
	}
private:
	float gear_ratio;
	float pos_ratio;
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_UV_MODULES_GEAR_RATIO_H_
