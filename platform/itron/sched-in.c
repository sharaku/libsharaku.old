/*
 * Copyright Abe Takafumi All Rights Reserved, 2004-2016
 * Author Abe Takafumi
 *
 */

////////////////////////////////////////////////////////////////////////
// itron, toppers / gcc

// itronは独自に以下の関数を呼び出す
// メインタスク
//  - sharaku_maintask_entry
// タイマー (1ms)
//  - sharaku_timer_entry
static volatile int sharaku_timer_flag = 0;

int sharaku_timer_entry(void)
{
	sharaku_timer_flag = 1;
}

int sharaku_maintask_entry(void)
{
	// sharakuの初期化
	sharaku_initialize();

	while (!sharaku_is_exit()) {
		if (sharaku_timer_flag) {
			sharaku_timer_handler();
			sharaku_timer_flag = 0;
		}
		sharaku_entry_job_cycle(sharaku_get_default_context());
	}

	// sharakuの破棄処理
	sharaku_finalize();

	return sharaku_get_exit_message();
}

