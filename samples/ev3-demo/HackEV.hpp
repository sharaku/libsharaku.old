/*****************************************************************************/
/*!
@author	abe takafumi
@par	Copyright
2014-2016 abe takafumi All Rights Reserved.
@file	
@brief	
******************************************************************************/

#include <stdio.h>
#include <devices/touch/ev3dev-touch.hpp>
#include <devices/gyro/ev3dev-gyro.hpp>
#include <devices/color/ev3dev-color.hpp>
#include <devices/usonic/ev3dev-ultrasonic.hpp>
#include <devices/motor/ev3dev-tacho-motor.hpp>
#include <devices/power/ev3dev-power-supply.hpp>
#include <devices/sd/sd-motors.hpp>
#include <devices/sd/sd-wheel-odometry.hpp>
#include <devices/sd/sd-position-move.hpp>
#include <modules/mod-update.hpp>

// モジュールの構成と接続関係
//      +------------------------------- _color
//      |
//      +------------------------------- _motor_t
//      |
//      +------------------------------- _motor_a
//      |
//      |             +- _motors  -+  +- _motor_r
// API -+- _pos_move -+            +--+
//                    +- _odo     -+  +- _motor_l
class HackEV
 : protected  sharaku::mod_update {
 public:
 	HackEV() :
 	mod_update(4),				// updateは4ms
	_power(50),				// 電源制御 50ms
	_touch(sharaku::EV3_INPUT_1, 50),	// IN3, 周期50ms
	_usonic(sharaku::EV3_INPUT_2, 25),	// IN4, 周期25ms
	_color(sharaku::EV3_INPUT_3, 4),	// IN2, 周期4ms
	_gyro(sharaku::EV3_INPUT_4, 4),		// IN1, 周期4ms
//	_motor_a(sharaku::EV3_OUTPUT_A, 8, sharaku::ev3dev_tacho_motor::MOTOR_MODE_SPEED),
//	_motor_t(sharaku::EV3_OUTPUT_D, 8, sharaku::ev3dev_tacho_motor::MOTOR_MODE_SPEED),
	_motor_r(sharaku::EV3_OUTPUT_B, 4, sharaku::ev3dev_tacho_motor::MOTOR_MODE_SPEED),
	_motor_l(sharaku::EV3_OUTPUT_C, 4, sharaku::ev3dev_tacho_motor::MOTOR_MODE_SPEED),
	_motors(180, 81),			// 車幅180mm, 車輪直径81mm
	_odo(180, 81)				// 車幅180mm, 車輪直径81mm
 	{
		printf("HackEV::HackEV()\n");
	}

	void initialize() {
		printf("HackEV::initialize\n");
		_gyro.reset();
		_color.set_mode(sharaku::ev3dev_color::EV3DEV_COLORMODE_CORRECTION);

		// 両輪を制御するmotorsモジュールに対して左右のモータを接続する
		_motors.out_speed_motor_l	= _motor_l;
		_motors.out_speed_motor_r	= _motor_r;

		_odo.in_speed_motor_l		= _motor_l;
		_odo.in_speed_motor_r		= _motor_r;

		_pos_move.in_odo		= _odo;
		_pos_move.out_move		= _motors;

		// 車両クラスに対して定期アップデートを登録する
		// デバイス類は自前で定期アップデートするため、登録不要
		register_update(_pos_move);
	}

	void start() {
		mod_update::start();
	}

	void stop() {
		mod_update::stop();
		_motor_r.set_speed_sp(0);
		_motor_l.set_speed_sp(0);
	}

	int32_t is_touch_press() {
		return _touch.is_press();
	}

	void set_move_ctrl(int speed, int steering) {
		_motors.set_speed_sp(speed);
		_motors.set_steer_sp(steering);
	}


 private:
	sharaku::ev3dev_powersupply	_power;		// 電源制御
	sharaku::ev3dev_gyro		_gyro;		// ジャイロセンサー
	sharaku::ev3dev_color		_color;		// カラーセンサー
	sharaku::ev3dev_touch		_touch;		// タッチセンサー
	sharaku::ev3dev_ultrasonic	_usonic;	// 超音波センサー
	sharaku::ev3dev_tacho_motor	_motor_r;	// モータ右
	sharaku::ev3dev_tacho_motor	_motor_l;	// モータ左
//	sharaku::ev3dev_tacho_motor	_motor_a;	// アームモータ
//	sharaku::ev3dev_tacho_motor	_motor_t;	// 尻尾モータ
	sharaku::sd_motors		_motors;
	sharaku::sd_position_move	_pos_move;
	sharaku::sd_wheel_odometry	_odo;
};

