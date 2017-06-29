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

#include <sharaku/fd.h>
#include <gtest/gtest.h>

static int _timer_cb_num;
void
timer_cb(void *_this)
{
	_timer_cb_num++;
}

TEST(fd, timerfd_sample) {
	struct timerfd_operations tfd_ope[] = {
		{timer_cb, NULL}
	};
	struct timerfd_info _timerinfo;
	struct fd_operations _fdope[] = {
		FD_OPERATIONS_INIT(timerfd_count_fd,
				timerfd_add_tfd,
				timerfd_do_fd, &_timerinfo)
	};

	_timer_cb_num = 0;
	
	timerfd_create_fd(&_timerinfo, 1000, tfd_ope, 1);
	fd_poll(_fdope, 1, -1);
	timerfd_destroy_fd(&_timerinfo);

	EXPECT_EQ(_timer_cb_num, 1);
}

#if 0
TEST(fd, inotify_sample) {
}

TEST(fd, signalfd_sample) {
}

TEST(fd, FD_OPERATIONS_INIT) {
}

TEST(fd, INIT_FD_OPERATIONS) {
}

TEST(fd, fd_poll) {
}

TEST(fd, timerfd_count_fd) {
}

TEST(fd, timerfd_add_tfd) {
}

TEST(fd, timerfd_do_fd) {
}

TEST(fd, inotify_count_fd) {
}

TEST(fd, inotify_add_fd) {
}

TEST(fd, inotify_do_fd) {
}

TEST(fd, signalfd_count_fd) {
}

TEST(fd, signalfd_add_fd) {
}

TEST(fd, signalfd_do_fd) {
}
#endif
