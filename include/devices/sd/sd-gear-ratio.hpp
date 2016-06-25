/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_UV_MODULES_GEAR_RATIO_H_
#define SHARAKU_UV_MODULES_GEAR_RATIO_H_

#include <stdint.h>
#include <devices/api.hpp>

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------

// ギア比補正をかけるモジュールです
// RATIOには、ギア比を指定します。
// タイヤ側3、モータ側1の場合、OUT_GEAR = 3, IN_GEAR = 1を指定します。
// タイヤ側1、モータ側8の場合、OUT_GEAR = 1, IN_GEAR = 8を指定します。
// ギアが偶数の場合で符号を反転させたい場合、OUT_GEAR側に-の値を指定してください。

template<int32_t OUT_GEAR, int32_t IN_GEAR>
class sd_gearratio_speed
 : virtual public speed_motor_operations
{
 public:
	// インターフェース接続(複数connect禁止)
	connection_interface<speed_motor_operations>	out_motor;
	const float 					gear_ratio;

	operator speed_motor_operations* () {
		return (speed_motor_operations*) this;
	}

 public:
	sd_gearratio_speed()
	 : gear_ratio((float)OUT_GEAR / (float)IN_GEAR) {
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
		return (int32_t)((float)pos * gear_ratio);
	}
	int32_t	set_position(int32_t pos) {
		int32_t	result = (int32_t)((float)pos * gear_ratio);
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
};

template<int32_t OUT_GEAR, int32_t IN_GEAR>
class sd_gearratio_angle
 : virtual public angle_motor_operations
{
 public:
	// インターフェース接続(複数connect禁止)
	connection_interface<angle_motor_operations>	out_motor;
	const float 					gear_ratio;

	operator angle_motor_operations* () {
		return (angle_motor_operations*) this;
	}

 public:
	sd_gearratio_angle()
	 : gear_ratio((float)OUT_GEAR / (float)IN_GEAR) {
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
		return (int32_t)((float)pos * gear_ratio);
	}
	int32_t	set_position(int32_t pos) {

		// ギヤ比補正をかける
		if ((angle_motor_operations*)out_motor) {
			int32_t	ratio_pos = (int32_t)((float)pos * gear_ratio);
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
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_UV_MODULES_GEAR_RATIO_H_
