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

#include <sharaku/sched.h>
#include <sharaku/task.h>

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
	syslog(LOG_NOTICE, "Start timer_task.");
	// 1ms周期で起動する
	while(1) {
		sharaku_timer_entry();
		tslp_tsk(1);
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
