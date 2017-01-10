/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#include <sharaku/sched.h>
#include "ev3way.hpp"
EV3way *_ev3way = NULL;

// ---------------------------------------------------------------------
// 定義
// ---------------------------------------------------------------------
static struct sharaku_job	_job_disp;

static void job_main(struct sharaku_job *job);
static void job_calib_black(struct sharaku_job *job);
static void job_calib_white(struct sharaku_job *job);
static void job_balancer_start(struct sharaku_job *job);
static void job_panel_cycle_r(struct sharaku_job *job);
static void job_panel_cycle_g(struct sharaku_job *job);
static void job_panel_cycle_o(struct sharaku_job *job);
static void start_disp();
static void job_disp(struct sharaku_job *job);

static void job_end(struct sharaku_job *job);
static void job_exit(struct sharaku_job *job);

// ---------------------------------------------------------------------
// メイン制御
// ---------------------------------------------------------------------
// メイン開始。
static void
job_main(struct sharaku_job *job)
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
static void
job_calib_black(struct sharaku_job *job)
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
static void
job_calib_white(struct sharaku_job *job)
{
	if (_ev3way->touch.is_push()) {
		_ev3way->linetrace.set_white(_ev3way->color.get_value());
		ev3_lcd_draw_string(" - complete: calib-white", 0, DISP_FONT_HEIGHT * 4);
		ev3_led_set_color(LED_RED);
		sharaku_async_message(job, job_balancer_start);
	} else {
		ev3_lcd_draw_string(" - check    : calib-white", 0, DISP_FONT_HEIGHT * 4);
		sharaku_timer_message(job, 100, job_calib_white);
	}
}

static void
job_balancer_start(struct sharaku_job *job)
{
	ev3_led_set_color(LED_RED);
	_ev3way->balancer.balancer_on();
	// バランサの起動を行ったので、ひとまずここまで。
#if 0
	// dispをキック
	// disp処理は重いため、バランサの4ms周期が守れなくなる。
	start_disp();
#endif

	job_panel_cycle_r(job);
	return;
}

// ---------------------------------------------------------------------
// パネルの色変更
// ---------------------------------------------------------------------
static void
job_panel_cycle_r(struct sharaku_job *job)
{
	ev3_led_set_color(LED_RED);
	sharaku_timer_message(job, 1000, job_panel_cycle_g);
}
static void
job_panel_cycle_g(struct sharaku_job *job)
{
	ev3_led_set_color(LED_GREEN);
	sharaku_timer_message(job, 1000, job_panel_cycle_o);
}
static void
job_panel_cycle_o(struct sharaku_job *job)
{
	ev3_led_set_color(LED_ORANGE);
	sharaku_timer_message(job, 1000, job_panel_cycle_r);
}

// ---------------------------------------------------------------------
// jobのエントリ
// ---------------------------------------------------------------------
static void
start_disp()
{
	sharaku_init_job(&_job_disp);
	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	sharaku_async_message(&_job_disp, job_disp);
}

static void
job_disp(struct sharaku_job *job)
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
void
job_entry(struct sharaku_job *job)
{ 

	// EV3wayを開始する
	_ev3way = new EV3way;
	_ev3way->start();
	ev3_led_set_color(LED_GREEN);	// 初期化完了通知
	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	ev3_lcd_draw_string("Sharaku-EV3way sample", 0, 1);

	sharaku_async_message(job, job_main);
}

static void
job_end(struct sharaku_job *job)
{
	_ev3way->stop();
	sharaku_timer_message(job, 100, job_exit);
}

static void
job_exit(struct sharaku_job *job)
{
	delete _ev3way;
}

// ---------------------------------------------------------------------
// バランサのサイクル処理
// ---------------------------------------------------------------------
void
balancer_cycle(void)
{
init_retry:
	tslp_tsk(10);
	if (!_ev3way) {
		goto init_retry;
	}

start_retry:
	tslp_tsk(4);
	if (!_ev3way->is_start()) {
		goto start_retry;
	}

retry:
	// 4ms周期でバランサ処理を呼び出す。
	// バランサ処理の中でgyro, motorのpos情報取得 * 2を行う。
	// この処理に大体1msかかるため、バランサに対する周期は5msとして
	// 設定する。
	//
	// balancer.hで0.00500000000Fに修正されている理由も上記と推測される。
	_ev3way->balancer.update(0.005f, 0);
	tslp_tsk(4);
	goto retry;
}
