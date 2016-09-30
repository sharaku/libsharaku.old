/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#include <stdio.h>
#include <devices/touch/ev3dev-touch.hpp>
#include <devices/gyro/ev3dev-gyro.hpp>
#include <devices/color/ev3dev-color.hpp>
#include <devices/usonic/ev3dev-ultrasonic.hpp>
#include <devices/motor/ev3dev-tacho-motor.hpp>
#include <devices/motor/dps2duty-motor.hpp>
#include <devices/power/ev3dev-power-supply.hpp>
#include <devices/sd/ev3way/ev3way-balancer.hpp>
#include <devices/sd/sd-wheel-odometry.hpp>
#include <devices/sd/sd-position-move.hpp>
#include <devices/sd/sd-linetrace-pid.hpp>
#include <modules/mod-update.hpp>


class EV3way
 : protected  sharaku::mod_update {
 public:
 	EV3way() :
 	mod_update(4),				// updateは4ms
	power(50),				// 電源制御 50ms
	touch(sharaku::EV3_INPUT_1, 50),	// IN3, 周期50ms
	usonic(sharaku::EV3_INPUT_2, 25),	// IN4, 周期25ms
	color(sharaku::EV3_INPUT_3, 4, sharaku::color_operations::MODE_REFLECTED),
						// IN2, 周期4ms
	gyro(sharaku::EV3_INPUT_4, 4),		// IN1, 周期4ms
	motor_t(sharaku::EV3_OUTPUT_D, 8, sharaku::ev3dev_tacho_motor::MOTOR_MODE_DUTY),
						// OUT D, 周期8ms
	motor_r(sharaku::EV3_OUTPUT_B, 4, sharaku::ev3dev_tacho_motor::MOTOR_MODE_DUTY),
						// OUT B, 周期4ms
	motor_l(sharaku::EV3_OUTPUT_C, 4, sharaku::ev3dev_tacho_motor::MOTOR_MODE_DUTY),
						// OUT C, 周期4ms
	linetrace(1.0f, 0.0f, 0.0f),		// PID無効
	target_move(81),			// 車輪直径81mm
	odo(180, 81)				// 車幅180mm, 車輪直径81mm
 	{
		// ----------------------------------------------------------
		// モジュールの結合を行う
		// ----------------------------------------------------------
		gyro.reset();

		// 両輪を制御するbalancerモジュールに対して左右のモータを接続する
		balancer.out_duty_motor_r	= motor_r;
		balancer.out_duty_motor_l	= motor_l;
		balancer.in_gyro		= gyro;
		balancer.in_power		= power;

		// ODOを接続
		spd2duty_motor_r.out_motor	= motor_r;
		spd2duty_motor_l.out_motor	= motor_l;
		odo.in_speed_motor_r		= spd2duty_motor_r;
		odo.in_speed_motor_l		= spd2duty_motor_l;

		// linetraceを接続
		linetrace.in_photo		= color;
		linetrace.out_move		= balancer;

		// 位置指定移動を接続
		target_move.in_odo		= odo;
		target_move.out_move		= linetrace;

		// ----------------------------------------------------------
		// 初期セッティング
		// ----------------------------------------------------------
		target_move.move_off();
		linetrace.trace_off();
		balancer.balancer_on();

		// ----------------------------------------------------------
		// 車両クラスに対して定期アップデートを登録する
		// デバイス類は自前で定期アップデートするため、登録不要
		// ----------------------------------------------------------
		register_update(odo);
		register_update(linetrace);
		register_update(target_move);
		register_update(balancer);
	}

	void start() {
		motor_r.reset();
		motor_l.reset();
		motor_t.reset();
		target_move.move_off();
		target_move.set_steer_sp(0);
		target_move.set_speed_sp(0);
		odo.reset();
		mod_update::start();
	}

	void stop() {
		motor_r.reset();
		motor_l.reset();
		motor_t.reset();
		target_move.move_off();
		target_move.set_steer_sp(0);
		target_move.set_speed_sp(0);
		odo.reset();
		mod_update::stop();
	}

 public:
	sharaku::ev3dev_powersupply	power;		// 電源制御
	sharaku::ev3dev_gyro		gyro;		// ジャイロセンサー
	sharaku::ev3dev_color		color;		// カラーセンサー
	sharaku::ev3dev_touch		touch;		// タッチセンサー
	sharaku::ev3dev_ultrasonic	usonic;		// 超音波センサー
	sharaku::ev3dev_tacho_motor	motor_r;	// モータ右
	sharaku::ev3dev_tacho_motor	motor_l;	// モータ左
	sharaku::ev3dev_tacho_motor	motor_t;	// 尻尾モータ
	sharaku::dps2duty_motor<1020>	spd2duty_motor_r;
	sharaku::dps2duty_motor<1020>	spd2duty_motor_l;
	sharaku::ev3way_balancer	balancer;
	sharaku::sd_linetrace_pid	linetrace;
	sharaku::sd_position_move	target_move;
	sharaku::sd_wheel_odometry	odo;
};

