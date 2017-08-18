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
#include <devices/sd/sd-linetrace.hpp>
#include <devices/sd/sd-gear-ratio.hpp>
#include <devices/sd/ev3way/ev3way-balancer.hpp>
#include <modules/mod-update.hpp>

// モジュールの構成と接続関係
//      +-------------------------------------------------- _motor_t
//      |
//      |                                      +- _gear_r - _motor_r
// -----+- _linetrace -+- _baranser - _motors -+
//                     |                       +- _gear_l - _motor_l
//                     +----------------------------------- _color
class EV3way 
 : protected sharaku::mod_update {
 public:
 	EV3way() :
 	sharaku::mod_update(4),			// updateは4ms
	_power(50),				// 電源制御 50ms
	_touch(sharaku::EV3_INPUT_1, 50),	// IN3, 周期50ms
	_usonic(sharaku::EV3_INPUT_2, 25),	// IN4, 周期25ms
	_color(sharaku::EV3_INPUT_3, 4),	// IN2, 周期4ms
	_gyro(sharaku::EV3_INPUT_4, 4),		// IN1, 周期4ms
	_motor_r(sharaku::EV3_OUTPUT_B, 4, sharaku::ev3dev_tacho_motor::MOTOR_MODE_DUTY),
	_motor_l(sharaku::EV3_OUTPUT_C, 4, sharaku::ev3dev_tacho_motor::MOTOR_MODE_DUTY)
//	_motor_t(sharaku::EV3_OUTPUT_A, 8, sharaku::ev3dev_tacho_motor::MOTOR_MODE_ANGLE),
 	{
		printf("EV3way::EV3way()\n");
	}

	void initialize() {
		printf("EV3way::initialize()\n");
		_gyro.reset();

		// 両輪を制御するmotorsモジュールに対して左右のモータを接続する
		_balancer.in_power		= _power;
		_balancer.in_gyro		= _gyro;
		_balancer.out_duty_motor_l	= _motor_l;
		_balancer.out_duty_motor_r	= _motor_r;

		// 車両クラスに対して定期アップデートを登録する
		// デバイス類は自前で定期アップデートするため、登録不要
		// _motorsは設定値をスルーするのみのため登録不要
		// _baranserは自前で定期アップデートするため、登録不要
		// _linetraceは定期的に動作する必要があるため、登録が必要
		register_update(_balancer);
	}

	void start() {
	}

	void stop() {
		mod_update::stop();
		_motor_r.set_duty_cycle_sp(0);
		_motor_l.set_duty_cycle_sp(0);
	}

	int32_t is_touch_press() {
		return _touch.is_press();
	}

	void set_move_ctrl(int speed, int steering) {
//		_motor_r.set_duty_cycle_sp(speed);
//		_motor_l.set_duty_cycle_sp(speed);
//		_balancer.set_speed_steer_sp(speed, steering);
		mod_update::start();
	}

 private:
	sharaku::ev3dev_powersupply	_power;		// 電源制御
	sharaku::ev3dev_touch		_touch;		// タッチセンサー
	sharaku::ev3dev_gyro		_gyro;		// ジャイロセンサー
	sharaku::ev3dev_color		_color;		// カラーセンサー
	sharaku::ev3dev_ultrasonic	_usonic;	// 超音波センサー
	sharaku::ev3dev_tacho_motor	_motor_l;	// モータ左
	sharaku::ev3dev_tacho_motor	_motor_r;	// モータ右
//	sharaku::ev3dev_tacho_motor	_motor_t;	// 尻尾モータ
	sharaku::ev3way_balancer	_balancer;	// バランサ
};

