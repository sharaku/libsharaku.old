/*
 * Copyright Abe Takafumi All Rights Reserved, 2017
 * Author Abe Takafumi
 *
 */

#include <sharaku/jiffies.h>
#include <sharaku/sched.h>
#include <sharaku/prof.h>
#include <sharaku/debug.h>

static volatile int static_exit_flag = 0;
static volatile int static_exit_code = 0;

static inline void
_sharaku_init(void)
{
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
sharaku_timer_handler(uint32_t cyc_ms)
{
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


