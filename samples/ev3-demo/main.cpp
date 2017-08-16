/*****************************************************************************/
/*!
@author	abe takafumi
@par	Copyright
2014-2015 abe takafumi All Rights Reserved.
@file	
@brief	
******************************************************************************/

#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sharaku/module.h>
#include <sharaku/sched.h>

//#define CONF_VEHICLE_TYPE_EV3way
//#define CONF_VEHICLE_TYPE_ETrikeV
#define CONF_VEHICLE_TYPE_HackEV

// 車両オブジェクトを生成する
#ifdef CONF_VEHICLE_TYPE_EV3way
#include "EV3way.hpp"
static EV3way		*_vehicle;
#endif
#ifdef CONF_VEHICLE_TYPE_ETrikeV
#include "ETrikeV.hpp"
static ETrikeV		*_vehicle;
#endif
#ifdef CONF_VEHICLE_TYPE_HackEV
#include "HackEV.hpp"
static HackEV		*_vehicle;
#endif

///////////////////////////////////////////////////////////////
// job構造体
static job_t	_job_exec;
static job_t	_job_exit;
static job_t	_job_timer;
static int			_stop_flag = 0;

///////////////////////////////////////////////////////////////
static void job_init_stage(job_t *job);
static void job_exec_stage1(job_t *job);
static void job_exec_stage2(job_t *job);
static void sample_sigint(int sig);
static void sample_exit(job_t *job);
static void sample_exit_end(job_t *job);
///////////////////////////////////////////////////////////////

static void
job_init_stage(job_t *job)
{
#ifdef CONF_VEHICLE_TYPE_EV3way
	_vehicle = new EV3way();
#endif
#ifdef CONF_VEHICLE_TYPE_ETrikeV
	_vehicle = new ETrikeV();
#endif
#ifdef CONF_VEHICLE_TYPE_HackEV
	_vehicle = new HackEV();
#endif
	_vehicle->initialize();

	// 開始
	printf("start.\n");
	_vehicle->start();

	// スタートまでのループを実行
	job_exec_stage1(job);
}

static void
job_exec_stage1(job_t *job)
{
	if(_stop_flag) {
		job_async_sched(&_job_exit, sample_exit);
		_stop_flag = 0;
	}

	// タッチセンサが押されるまで待つ
	// 押された場合は、尻尾を収納してバランサモードへ遷移する
	if (_vehicle->is_touch_press()) {
#ifdef CONF_VEHICLE_TYPE_EV3way
		// EV3wayは走行開始時に尻尾を上げる
//		_vehicle->tail_off();
#endif
		// linetraceをONにする。
//		_vehicle->trace_on();

		// 速度を30に設定する
		_vehicle->set_move_ctrl(500, -45);

		// 走行ループに移行する
		job_async_sched(job, job_exec_stage2);
	} else {
		// ボタンが押されていない間は、50ms間隔でループする
		job_timer_sched(job, 50, job_exec_stage1);
	}
}

static void
job_exec_stage2(job_t *job)
{
	if(_stop_flag) {
		job_async_sched(&_job_exit, sample_exit);
		_stop_flag = 0;
	}
	// 無限に待ち合わせる
	// スケジュールしなくても問題ないが、処理を追加しやすいように
	// ステージ化してループするようにしておく
	job_timer_sched(job, 50, job_exec_stage2);
}

static void
sample_sigint(int sig)
{
	// 割り込み契機なので、flagのみ
	_stop_flag = 1;
}

static void
sample_exit(job_t *job)
{
	// スケジュールされているjobをキャンセルする
	job_cancel_sched(&_job_exec);

	// 車両の停止を行う
	_vehicle->stop();
	init_job(&_job_exit);

	// 終了するまで100ms待つ。その間にモータ等を終了させる
	sharaku_timer_message(&_job_exit, 100, sample_exit_end);
}

static void
sample_exit_end(job_t *job)
{
	// 終了する。
	// これにより、sharaku_entryが終了する。
	sharaku_exit_message(0);
}

// メインループ
// 基本はこの形
int
main(void)
{
	int	result;

	// 周期を守るために周りのプロセスよりも高い優先度(-5)へ設定する。
	setpriority(PRIO_PGRP, 0, -5);

	// スケジュール
	if (signal(SIGINT, sample_sigint) == SIG_ERR) {
		return 0;
	}

	init_job(&_job_exec);
	init_job(&_job_exit);
	job_async_sched(&_job_exec, job_init_stage);
	result = sharaku_entry();

	return result;
}
