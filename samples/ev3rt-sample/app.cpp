/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */
 
#include "ev3api.h"
extern "C" {
#include "app.h"
}

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

extern "C" void job_entry(struct sharaku_job *job);

#include <sharaku/utime.h>
#include <sharaku/jiffies.h>
#include <sharaku/sched.h>
#include <sharaku/system.h>

// ---------------------------------------------------------------------
// 定義
// ---------------------------------------------------------------------
static struct sharaku_job	_job_init;

// ---------------------------------------------------------------------
// タスクエントリ
// ---------------------------------------------------------------------
void idle_task(intptr_t unused)
{
	while(1) {
		tslp_tsk(1000);
	}
}

void
timer_task(intptr_t unused) {
	int64_t	start_us, now_ms, now_us,diff_ms;
	start_us = sharaku_get_usec();
	// 1ms周期で起動する
	while(1) {
		tslp_tsk(2);
		now_us = sharaku_get_usec();
		now_ms = sharaku_get_msec();
		diff_ms = (now_us - start_us) / 1000 - now_ms;
		sharaku_timer_handler(diff_ms);
	}
}

void main_task(intptr_t unused)
{
	// Start task for timer
	act_tsk(TIMER_TASK);

	// Start task for printing message while idle
	act_tsk(IDLE_TASK);

	ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */
	sharaku_init_job(&_job_init);
	sharaku_async_message(&_job_init, job_entry);

	sharaku_entry();
}
