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

#include <sharaku/jiffies.h>
#include <sharaku/sched.h>
#include <sharaku/prof.h>
#include <sharaku/debug.h>

static volatile int static_exit_flag = 0;
static volatile int static_exit_code = 0;
static sharaku_usec_t static_start_us = 0;

static inline void
_sharaku_init(void)
{
	static_start_us = sharaku_get_usec();

	sharaku_init_dblog();
	sharaku_init_prof();
	sharaku_init_sched();
}

static inline void
_sharaku_finl(void)
{
	sharaku_finl_sched();
	sharaku_finl_prof();
	sharaku_finl_dblog();
}

static inline int
_is_exit(void)
{
	return static_exit_flag;
}

void
sharaku_timer_handler(void)
{
	int64_t	now_ms, now_us, cyc_ms;

	// start_us が now_ms = 0の情報。
	// 差分は、(now_us - start_us) / 1000
	now_us = sharaku_get_usec();
	now_ms = sharaku_get_msec();
	cyc_ms = (now_us - static_start_us) / 1000 - now_ms;

	// 周期呼び出しにより、jiffiseを更新する。
	jiffies += cyc_ms;
}

void
sharaku_cpu_handler(void)
{
	while (!_is_exit()) {
		sharaku_workqueue();
	}
}

// システムのエントリポイント
void
sharaku_exit_message(int result)
{
	static_exit_code = result;
	static_exit_flag = 1;
}

int
sharaku_get_exit_message(void)
{
	return static_exit_code;
}


// include先は、OS, プラットフォーム, アーキテクチャによって
// 適切に切り替わる。
#include <system-in.c>


