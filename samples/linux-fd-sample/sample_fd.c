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
#include <sys/timerfd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/timerfd.h>
#include <sharaku/sched.h>
#include <sharaku/system.h>
#include <sharaku/utime.h>
#include <sharaku/jiffies.h>
#include <sharaku/fd.h>

job_t	_job;
job_t	_timer_job;

uint32_t count = 0;
uint32_t old_us = 0;
uint32_t old_ms = 0;


int _timerfd;

static int
_timerfd_count_fd(void)
{
	return 1;
}

static int
_timerfd_add_fd(struct pollfd *pfd, int cnt)
{
	pfd[0].fd = _timerfd;
	pfd[0].events = POLLIN;
	pfd[0].revents = 0;
	return 1;
}

static int
_timerfd_do_fd(struct pollfd *pfd, int cnt)
{
	uint64_t	work;

	if (pfd[0].revents & POLLIN) {
		read(_timerfd, &work, sizeof work);
		printf("  time out!!\n");
	}
	return 1;
}

struct fd_operations	_ops[] = {
	{_timerfd_count_fd, _timerfd_add_fd, _timerfd_do_fd}
};

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

	// 登録されているFDを監視する。
	// タイムアウトに0を指定することで即時応答する。
	// sharaku jobと組み合わせる場合は0とすること。
	fd_poll(_ops, 1, 0);

	job_async_sched(job, sched_cb);
}

static void
init_cb(job_t *job)
{
	// タイマーの作成。
	struct itimerspec iti;
	iti.it_value.tv_sec	= 1;
	iti.it_value.tv_nsec	= 0;
	iti.it_interval.tv_sec	= 2;
	iti.it_interval.tv_nsec	= 0;

	_timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
	timerfd_settime(_timerfd, TFD_TIMER_ABSTIME, &iti, NULL);

	job_async_sched(job, sched_cb);
}

int
main(void)
{
	old_us = sharaku_get_usec();

	printf("<now us    ,interval us,now ms     ,interval ms>: count\n");
	init_job(&_job);
	init_job_prio(&_timer_job, 0);
	job_async_sched(&_job, init_cb);
	job_async_sched(&_timer_job, timer_sched_cb);
	sharaku_entry();
}

