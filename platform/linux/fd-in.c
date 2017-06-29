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
#include <unistd.h>
#include <sharaku/fd.h>

int
timerfd_create_fd(struct timerfd_info *_this, uint32_t interval_ms,
		struct timerfd_operations *ops, int opsnum)
{
	int fd;
	int rc;
	struct itimerspec its;

	its.it_interval.tv_sec	= interval_ms / 1000;
	its.it_interval.tv_nsec	= (interval_ms % 1000) * 1000000;
	its.it_value.tv_sec	= interval_ms / 1000;
	its.it_value.tv_nsec	= (interval_ms % 1000) * 1000000;

	fd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (fd < 0) {
		return -1;
	}
	rc = timerfd_settime(fd, 0, &its, NULL);
	if (rc < 0) {
		close(fd);
		return -1;
	}

	_this->t_fd		= fd;
	_this->t_ms		= interval_ms;
	_this->t_ops_num	= opsnum;
	_this->t_ops		= ops;

	return 0;
}

int
timerfd_destroy_fd(struct timerfd_info *_this)
{
	close(_this->t_fd);
	_this->t_fd		= -1;
	_this->t_ms		= 0;
	_this->t_ops_num	= 0;
	_this->t_ops		= NULL;
	return 0;
}

int
timerfd_count_fd(void *_this)
{
	struct timerfd_info *ti = (struct timerfd_info*)_this;

	if (ti->t_fd == -1) {
		return 0;
	} else {
		return 1;
	}
}

int
timerfd_add_tfd(void *_this, struct pollfd *pfd, int cnt)
{
	struct timerfd_info *ti = (struct timerfd_info*)_this;

	if (ti->t_fd == -1) {
		return 0;
	} else {
		pfd[0].fd	= ti->t_fd;
		pfd[0].events	= POLLIN;
		pfd[0].revents	= 0;
		return 1;
	}
}

int
timerfd_do_fd(void *_this, struct pollfd *pfd, int cnt)
{
	struct timerfd_info *ti = (struct timerfd_info*)_this;
	uint64_t fdcnt = 0;
	int i;

	if (pfd[0].revents & POLLIN) {
		(void)read(ti->t_fd, &fdcnt, sizeof fdcnt);
		// pollしているので、fdcntが0ということはあり得ないが、
		// 念のためガードを入れる。
		if (fdcnt > 0) {
			for (i = 0; i < ti->t_ops_num; i++) {
				ti->t_ops[i].t_cb(ti->t_ops[i]._this);
			}
		}
		return cnt;
	} else if (pfd[0].revents & (POLLIN | POLLERR | POLLHUP | POLLNVAL)) {
		return -1;
	}
	return cnt;
}

#if 0
int
inotify_count_fd(void *_this)
{
}

int
inotify_add_fd(void *_this, struct pollfd *pfd, int cnt)
{
}

int
inotify_do_fd(void *_this, struct pollfd *pfd, int cnt)
{
}

int
signalfd_count_fd(void *_this)
{
}

int
signalfd_add_fd(void *_this, struct pollfd *pfd, int cnt)
{
}

int
signalfd_do_fd(void *_this, struct pollfd *pfd, int cnt)
{
}

#endif
