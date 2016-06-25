/*****************************************************************************/
/*!
@author	abe takafumi
@par	Copyright
2008-2014 abe takafumi All Rights Reserved.
@addtogroup core
@file	module.c
@brief	sharakuのmodule機能
	対応プラットフォーム：Windows (C++), Linux (C/C++) ...
******************************************************************************/

/******************************************************************************
include section
******************************************************************************/
#include <stdint.h>
#include <sharaku/task.h>
#include <sharaku/module.h>

static struct list_head	static_module_list = LIST_HEAD_INIT(static_module_list);

void sharaku_module_regist(struct sharaku_module *module)
{
	// priority順にソートしながら登録する
	// 未実装
	list_add_tail(&(module->list), &static_module_list);
}

static void sharaku_module_cycle_stage(struct sharaku_job *module_job)
{
	struct sharaku_module	*entry;

	entry = list_entry(module_job, struct sharaku_module, job);
	entry->cycle();
	sharaku_timer_message(module_job, entry->interval, sharaku_module_cycle_stage);
}

void sharaku_module_initialize()
{
	struct sharaku_module	*entry;
	list_for_each_entry(entry, &static_module_list, struct sharaku_module, list) {
		if (entry->init) {
			entry->init();
		}
	}

	// 一気にスケジュールを行う
	list_for_each_entry(entry, &static_module_list, struct sharaku_module, list) {
		if (entry->cycle) {
			sharaku_timer_message(&(entry->job), entry->interval, sharaku_module_cycle_stage);
		}
	}
}

void sharaku_module_finalize()
{
	struct sharaku_module	*entry;
	
	// 全moduleのスケジュールを停止する
	// スケジュールされていない場合は何もされない
	list_for_each_entry_reverse(entry, &static_module_list, struct sharaku_module, list) {
		sharaku_cancel_message(&(entry->job));
	}

	// 登録の逆順で破棄を行う
	list_for_each_entry_reverse(entry, &static_module_list, struct sharaku_module, list) {
		if (entry->final) {
			entry->final();
		}
	}
}

