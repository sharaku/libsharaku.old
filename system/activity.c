/*****************************************************************************/
/*!
@author	abe takafumi
@par	Copyright
2008-2014 abe takafumi All Rights Reserved.
@addtogroup core
@file	activity.c
@brief	sharakuのactivity機能
	対応プラットフォーム：Windows (C++), Linux (C/C++) ...
******************************************************************************/

/******************************************************************************
include section
******************************************************************************/
#include <stdint.h>
#include <sharaku/task.h>
#include <sharaku/activity.h>
#include <sharaku/sched.h>

static struct list_head	activity_list = LIST_HEAD_INIT(activity_list);
static struct list_head	current_activity_list = LIST_HEAD_INIT(current_activity_list);
static struct sw_job {
	struct sharaku_job job;
	struct sharaku_activity *crrent;
	struct sharaku_activity *next;
	uint8_t type;
} activity_switch_job;


static void sharaku_activity_cycle_stage(struct sharaku_job *activity_job);
static void sharaku_activity_single_sw_stage(struct sharaku_job *job);

// activityへメッセージを送信する
void sharaku_activity_message(struct sharaku_activity *entry, int message)
{
	sharaku_activity_stagefunc_t	message_stage;
	message_stage = entry->message_stage[message];

	if (message_stage) {
		message_stage();
	}

	// message毎にstatusの変更を行う
	switch (message) {
	case SHARAKU_ACTIVITY_MSG_CREATE:
		entry->status = SHARAKU_ACTIVITY_STATUS_RESERVE;
		break;
	case SHARAKU_ACTIVITY_MSG_START:
		break;
	case SHARAKU_ACTIVITY_MSG_RESUME:
		entry->status = SHARAKU_ACTIVITY_STATUS_ACTIVE;
			if (entry->interval) {
				sharaku_async_message(&(entry->job), sharaku_activity_cycle_stage);
			}
		break;
	case SHARAKU_ACTIVITY_MSG_PAUSE:
		sharaku_cancel_message(&(entry->job));
		entry->status = SHARAKU_ACTIVITY_STATUS_SUSPEND;
		break;
	case SHARAKU_ACTIVITY_MSG_STOP:
		sharaku_cancel_message(&(entry->job));
		entry->status = SHARAKU_ACTIVITY_STATUS_STOP;
		break;
	case SHARAKU_ACTIVITY_MSG_DESTROY:
		entry->status = SHARAKU_ACTIVITY_STATUS_IDLE;
		break;
	case SHARAKU_ACTIVITY_MSG_RESTART:
		break;
	case SHARAKU_ACTIVITY_MSG_ROUTINE:
		break;
	}
}

// activityを登録する
void sharaku_activity_regist(struct sharaku_activity *activity)
{
	list_add_tail(&(activity->list), &activity_list);
}

// activityのcycle処理
static void sharaku_activity_cycle_stage(struct sharaku_job *activity_job)
{
	struct sharaku_activity	*entry = list_entry(activity_job, struct sharaku_activity, job);
	sharaku_activity_message(entry, SHARAKU_ACTIVITY_MSG_ROUTINE);
	sharaku_timer_message(activity_job, entry->interval, sharaku_activity_cycle_stage);
}

// activityの初期化
void sharaku_activity_initialize(void)
{
	struct sharaku_activity	*entry;
	struct sharaku_activity	*current = NULL;

	sharaku_init_job(&(activity_switch_job.job));

	// 起動時にすべてのActivityをCreateする
	list_for_each_entry(entry, &activity_list, struct sharaku_activity, list) {
		sharaku_activity_message(entry, SHARAKU_ACTIVITY_MSG_CREATE);
	}

	// current activityを検索する
	list_for_each_entry(entry, &activity_list, struct sharaku_activity, list) {
		if (entry->current) {
			current = entry;
			break;
		}
	}
	if (current == NULL) {
		return;
	}

	// activityの起動を行う。
	// 起動は、start -> resume -> routine ...の順とする
	list_add_tail(&(current->active_list), &current_activity_list);
	sharaku_activity_message(entry, SHARAKU_ACTIVITY_MSG_START);
	sharaku_activity_message(entry, SHARAKU_ACTIVITY_MSG_RESUME);
}

// activityの破棄処理
void sharaku_activity_finalize(void)
{
	struct sharaku_activity	*entry;

	// 全moduleのスケジュールを停止する
	// スケジュールされていない場合は何もされない
	list_for_each_entry(entry, &current_activity_list, struct sharaku_activity, active_list) {
	if (!((entry->status == SHARAKU_ACTIVITY_STATUS_STOP) ||
	      (entry->status == SHARAKU_ACTIVITY_STATUS_IDLE))) {
			sharaku_activity_message(entry, SHARAKU_ACTIVITY_MSG_STOP);
			sharaku_cancel_message(&(entry->job));
		}
	}
	
	// 登録の逆順で破棄を行う
	list_for_each_entry(entry, &activity_list, struct sharaku_activity, list) {
		if (!(entry->status == SHARAKU_ACTIVITY_STATUS_IDLE)) {
			sharaku_activity_message(entry, SHARAKU_ACTIVITY_MSG_DESTROY);
		}
	}
}

// activityを切り替える
void sharaku_activity_start(uint32_t id, uint8_t type, uint8_t sw)
{
	struct sharaku_activity	*entry;
	struct sharaku_activity	*next = NULL;

	// currentとnextを取得する
	list_for_each_entry(entry, &activity_list, struct sharaku_activity, list) {
		if (entry->id == id) {
			next = entry;
			break;
		}
	}
	if (next == NULL) {
		return;
	}

	// swに応じて切り替えを起動する
	activity_switch_job.crrent = list_last_entry(&current_activity_list, struct sharaku_activity, active_list);
	activity_switch_job.next   = next;
	activity_switch_job.type   = type;
	sharaku_async_message(&(activity_switch_job.job), sharaku_activity_single_sw_stage);
}

// 元のactivityへ戻る
void sharaku_activity_finish(void)
{
	// swに応じて切り替えを起動する
	activity_switch_job.crrent = list_last_entry(&current_activity_list, struct sharaku_activity, active_list);
	activity_switch_job.next   = list_first_entry(&current_activity_list, struct sharaku_activity, active_list);
	if (activity_switch_job.crrent != activity_switch_job.next) {
		activity_switch_job.type = SHARAKU_ACTIVITY_TYPE_ACTIVITY_FINISH;
	} else {
		activity_switch_job.type = SHARAKU_ACTIVITY_TYPE_ACTIVITY_EXIT;
	}
	sharaku_async_message(&(activity_switch_job.job), sharaku_activity_single_sw_stage);
}

// 単純なactivity切り替え
static void sharaku_activity_single_sw_stage(struct sharaku_job *job)
{
	struct sharaku_activity	*entry;
	struct sharaku_activity	*n;
	struct sw_job   *sw_job;

	sw_job = list_entry(job, struct sw_job, job);

	// flag指定によって動作を変える
	// SHARAKU_ACTIVITY_FLAG_ACTIVITY_NONの場合はstackに積む
	// SHARAKU_ACTIVITY_FLAG_ACTIVITY_CLEAR_TOPの場合は総てをStopする
	// SHARAKU_ACTIVITY_TYPE_ACTIVITY_EXITの場合は総てをStopする
	// SHARAKU_ACTIVITY_TYPE_ACTIVITY_FINISHの場合はcurrentをStopする
	if (sw_job->type == SHARAKU_ACTIVITY_TYPE_ACTIVITY_NON) {
		sharaku_activity_message(sw_job->crrent, SHARAKU_ACTIVITY_MSG_PAUSE);
	} else if ((sw_job->type == SHARAKU_ACTIVITY_TYPE_ACTIVITY_CLEAR_TOP) ||
		   (sw_job->type == SHARAKU_ACTIVITY_TYPE_ACTIVITY_EXIT)) {
		list_for_each_entry_safe(entry, n, &current_activity_list, struct sharaku_activity, active_list) {
			sharaku_activity_message(entry, SHARAKU_ACTIVITY_MSG_STOP);
			list_del_init(&(entry->active_list));
		}
	} else if (sw_job->type == SHARAKU_ACTIVITY_TYPE_ACTIVITY_FINISH) {
		sharaku_activity_message(sw_job->crrent, SHARAKU_ACTIVITY_MSG_STOP);
		list_del_init(&(sw_job->crrent->active_list));
	}

	if (sw_job->type == SHARAKU_ACTIVITY_TYPE_ACTIVITY_EXIT) {
		sharaku_exit_message(0);
		return;
	}

	// 遷移先を起動する
	if (sw_job->type == SHARAKU_ACTIVITY_TYPE_ACTIVITY_FINISH) {
		sharaku_activity_message(sw_job->next, SHARAKU_ACTIVITY_MSG_RESTART);
		sharaku_activity_message(sw_job->next, SHARAKU_ACTIVITY_MSG_RESUME);
	} else {
		list_add_tail(&(sw_job->next->active_list), &current_activity_list);
		sharaku_activity_message(sw_job->next, SHARAKU_ACTIVITY_MSG_START);
		sharaku_activity_message(sw_job->next, SHARAKU_ACTIVITY_MSG_RESUME);
	}
}
