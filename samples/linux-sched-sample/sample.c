/* --
 *
 * MIT License
 * 
 * Copyright (c) 2017 Abe Takafumi
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
#include <stdlib.h>
#include <stdio.h>
#include <sharaku/sched.h>
#include <sharaku/system.h>
#include <sharaku/utime.h>
#include <sharaku/jiffies.h>

job_t	_job;
job_t	_timer_job;

uint32_t count = 0;
uint32_t old_us = 0;
uint32_t old_ms = 0;

static void
timer_sched_cb(job_t *job)
{
	uint32_t us = sharaku_get_usec();
	uint32_t ms = sharaku_get_msec();
	job_timer_sched(job, 1000, timer_sched_cb);
	printf("<%-10u, %-10u, %-10u, %-10u>: %-10u\n", us, us - old_us, ms, ms - old_ms, count);
	old_us = us;
	old_ms = ms;
}

static void
sched_cb(job_t *job)
{
	count ++;
	job_async_sched(job, sched_cb);
}

int
main(void)
{
	old_us = sharaku_get_usec();

	printf("<now us    ,interval us,now ms     ,interval ms>: count\n");
	init_job(&_job);
	init_job_prio(&_timer_job, 0);
	job_async_sched(&_job, sched_cb);
	job_async_sched(&_timer_job, timer_sched_cb);
	sharaku_entry();
}

