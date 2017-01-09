/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#include <sharaku/sched.h>
#include "ev3way.hpp"
EV3way *_ev3way;

// ---------------------------------------------------------------------
// 定義
// ---------------------------------------------------------------------
static struct sharaku_job	_job_disp;
static struct sharaku_job	_job_sleep;

static void job_main(struct sharaku_job *job);
static void job_calib_black(struct sharaku_job *job);
static void job_calib_white(struct sharaku_job *job);
static void job_chk_lmotor(struct sharaku_job *job);
static void job_chk_rmotor(struct sharaku_job *job);
static void job_chk_gyro(struct sharaku_job *job);
static void job_balancer_start(struct sharaku_job *job);
static void job_balancer(struct sharaku_job *job);
static void job_end(struct sharaku_job *job);
static void job_exit(struct sharaku_job *job);
static void start_disp();
static void job_disp(struct sharaku_job *job);
static void start_sleep();
static void job_sleep(struct sharaku_job *job);

// ---------------------------------------------------------------------
// メイン制御
// ---------------------------------------------------------------------
// メイン開始。
static void job_main(struct sharaku_job *job)
{
	static int exec = 0;
	if (!exec) {
		_ev3way->gyro.reset();
		exec ++;
		sharaku_timer_message(job, 1000, job_main);
		return;
	}

	ev3_lcd_draw_string(" - start", 0, DISP_FONT_HEIGHT * 2);
	sharaku_async_message(job, job_calib_black);
}

// キャリブレーション黒
static void job_calib_black(struct sharaku_job *job)
{
	if (_ev3way->touch.is_push()) {
		_ev3way->linetrace.set_black(_ev3way->color.get_value());
		ev3_lcd_draw_string(" - complete: calib-black", 0, DISP_FONT_HEIGHT * 3);
		ev3_led_set_color(LED_ORANGE);
		sharaku_async_message(job, job_calib_white);
	} else {
		ev3_lcd_draw_string(" - check    : calib-black", 0, DISP_FONT_HEIGHT * 3);
		sharaku_timer_message(job, 100, job_calib_black);
	}
}

// キャリブレーション白
static void job_calib_white(struct sharaku_job *job)
{
	if (_ev3way->touch.is_push()) {
		_ev3way->linetrace.set_white(_ev3way->color.get_value());
		ev3_lcd_draw_string(" - complete: calib-white", 0, DISP_FONT_HEIGHT * 4);
		ev3_led_set_color(LED_RED);
#if 0
		sharaku_async_message(job, job_chk_lmotor);
#else
		sharaku_async_message(job, job_balancer_start);
#endif
	} else {
		ev3_lcd_draw_string(" - check    : calib-white", 0, DISP_FONT_HEIGHT * 4);
		sharaku_timer_message(job, 100, job_calib_white);
	}
}

static void job_chk_lmotor(struct sharaku_job *job)
{
	static int exec = 0;
	if (exec == 0) {
		_ev3way->motor_l.set_duty_cycle_sp(100);
	}
	if (exec == 1) {
		_ev3way->motor_l.set_duty_cycle_sp(-100);
	}
	if (_ev3way->touch.is_push()) {
		exec ++;
		_ev3way->motor_l.set_duty_cycle_sp(0);
		if (exec < 2) {
			sharaku_timer_message(job, 100, job_chk_lmotor);
			return;
		}
		ev3_lcd_draw_string(" - complete: l-motor", 0, DISP_FONT_HEIGHT * 5);

		sharaku_async_message(job, job_chk_rmotor);
	} else {
		ev3_lcd_draw_string(" - check    : l-motor", 0, DISP_FONT_HEIGHT * 5);
		sharaku_timer_message(job, 100, job_chk_lmotor);
	}
}

static void job_chk_rmotor(struct sharaku_job *job)
{
	static int exec = 0;
	if (exec == 0) {
		_ev3way->motor_r.set_duty_cycle_sp(100);
	}
	if (exec == 1) {
		_ev3way->motor_r.set_duty_cycle_sp(-100);
	}
	if (_ev3way->touch.is_push()) {
		exec ++;
		_ev3way->motor_r.set_duty_cycle_sp(0);
		if (exec < 2) {
			sharaku_timer_message(job, 100, job_chk_lmotor);
			return;
		}
		ev3_lcd_draw_string(" - complete: r-motor", 0, DISP_FONT_HEIGHT * 6);
		sharaku_async_message(job, job_balancer_start);
	} else {
		ev3_lcd_draw_string(" - check    : r-motor", 0, DISP_FONT_HEIGHT * 6);
		sharaku_timer_message(job, 100, job_chk_rmotor);
	}
}

#include "balancer.h"

static uint32_t static_time = 0;
static void job_balancer_start(struct sharaku_job *job)
{
	ev3_led_set_color(LED_RED);
	_ev3way->balancer.balancer_on();
	static_time = sharaku_get_usec() - 4000;
	job_balancer(job);
}
static void job_balancer(struct sharaku_job *job)
{
	int32_t motor_ang_l, motor_ang_r;
	int32_t gyro, volt;
	int8_t pwm_L, pwm_R;

	if ((sharaku_get_usec() - static_time) < 4000) {
		sharaku_async_message(job, job_balancer);
		return;
	}


#if 0
	float interval = 0.004f;
	_ev3way->balancer.update(interval, 0);
#else
	motor_ang_l = _ev3way->motor_l.get_position();
	motor_ang_r = _ev3way->motor_r.get_position();
	gyro = _ev3way->gyro.get_rate();
	volt = _ev3way->power.get_voltage();
	balance_control(
		(float)0.0f,
		(float)0.0f,
		(float)gyro,
		(float)0,
		(float)motor_ang_l,
		(float)motor_ang_r,
		(float)volt,
		(int8_t *)&pwm_L,
		(int8_t *)&pwm_R);
	_ev3way->motor_l.set_duty_cycle_sp(pwm_L);
	_ev3way->motor_r.set_duty_cycle_sp(pwm_R);
#endif
	static_time = sharaku_get_usec();

	sharaku_async_message(job, job_balancer);
}

// ---------------------------------------------------------------------
// jobのエントリ
// ---------------------------------------------------------------------
static void start_disp()
{
	sharaku_init_job(&_job_disp);
	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	sharaku_async_message(&_job_disp, job_disp);
}

static void job_disp(struct sharaku_job *job)
{
	static int disp_id = 0;
	if (ev3_button_is_pressed(LEFT_BUTTON)) {
		disp_id = 0;
		ev3_lcd_fill_rect(0, 0,
				EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	}
	if (ev3_button_is_pressed(RIGHT_BUTTON)) {
		disp_id = 1;
		ev3_lcd_fill_rect(0, 0,
				EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	}

	switch (disp_id) {
	case 0:
		sharaku_dev_disp(_ev3way);
		break;
	case 1:
		sharaku_prof_disp(_ev3way);
		break;
	}

	sharaku_timer_message(job, 100, job_disp);
}

// ---------------------------------------------------------------------
// jobのエントリ
// ---------------------------------------------------------------------
void job_entry(struct sharaku_job *job)
{ 

	// EV3wayを開始する
	_ev3way = new EV3way;
	_ev3way->start();
	ev3_led_set_color(LED_GREEN);	// 初期化完了通知
	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	ev3_lcd_draw_string("Sharaku-EV3way sample", 0, 1);

	sharaku_async_message(job, job_main);
}

static void job_end(struct sharaku_job *job)
{
	_ev3way->stop();
	sharaku_timer_message(job, 100, job_exit);
}

static void job_exit(struct sharaku_job *job)
{
	delete _ev3way;
}
