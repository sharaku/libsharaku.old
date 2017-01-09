/*
 * Copyright Abe Takafumi All Rights Reserved, 2004-2016
 * Author Abe Takafumi
 *
 */
#include "ev3api.h"
#include "app.h"

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

////////////////////////////////////////////////////////////////////////
// itron, toppers / gcc

// itronは独自に以下の関数を呼び出す
// メインタスク
//  - sharaku_entry
// タイマー (1ms)
//  - sharaku_timer_handler(1);
int sharaku_entry(void)
{
	_sharaku_init();
	sharaku_cpu_handler();
	_sharaku_finl();
	return sharaku_get_exit_message();
}

