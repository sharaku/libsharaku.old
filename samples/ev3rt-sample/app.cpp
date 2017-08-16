/* --
 *
 * MIT License
 * 
 * Copyright (c) 2016-2017 Abe Takafumi
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
 * SOFTWARE.
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

extern void job_entry(job_t *job);
extern void balancer_cycle(void);

#include <sharaku/utime.h>
#include <sharaku/jiffies.h>
#include <sharaku/sched.h>
#include <sharaku/system.h>

// ---------------------------------------------------------------------
// 定義
// ---------------------------------------------------------------------
static job_t	_job_init;

// ---------------------------------------------------------------------
// タスクエントリ
// ---------------------------------------------------------------------
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

void cycle_task(intptr_t unused)
{
	balancer_cycle();
}

void main_task(intptr_t unused)
{
	// Start task for timer
	act_tsk(TIMER_TASK);

	// Start task for printing message while idle
	act_tsk(CYCLE_TASK);

	ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */
	init_job(&_job_init);
	job_async_sched(&_job_init, job_entry);

	sharaku_entry();
}
