/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_MODULES_MOD_UPDATE_H_
#define SHARAKU_MODULES_MOD_UPDATE_H_

#include <stdint.h>
#include <sharaku/task.h>
#include <sharaku/list.h>
#include <devices/update-api.hpp>

NAMESPACE_SHARAKU_BEGIN

// mdsetup addのようなもので登録する。
// 登録したものに対してUpdateイベントを発行する、
// 登録時に優先度を設定することで、uodate優先度を設定する。

// 登録されたモジュールにUpdate通知を発行する
// また、定期的に出力情報をポーリングする
class mod_update
 : public update_operations
{
 public:
	mod_update(uint32_t interval);

	// Updateの登録
	void register_update(update_operations* update);

	// Updateの開始と停止
	void start();
	void stop();

 protected:
	static void mod_update_cycle(struct sharaku_job* job);
	static void mod_pre_update_begin(struct sharaku_job* job);
	static void mod_pre_update(struct sharaku_job* job);
	static void mod_pre_update_retry(struct sharaku_job* job);
	static void mod_pre_update_end(struct sharaku_job* job);
	static void mod_update_begin(struct sharaku_job* job);
	static void mod_update_(struct sharaku_job* job);
	static void mod_update_retry(struct sharaku_job* job);
	static void mod_update_end(struct sharaku_job* job);
	static void mod_post_update_begin(struct sharaku_job* job);
	static void mod_post_update(struct sharaku_job* job);
	static void mod_post_update_retry(struct sharaku_job* job);
	static void mod_post_update_end(struct sharaku_job* job);

 private:
	mod_update();

 private:
	sharaku_usec_t		_time;		// 前回実行時の時間

	struct sharaku_job	_job_update;
	struct sharaku_job	_job_update_interval;
	struct sharaku_job	*_job_update_i;
	update_operations	*_update;

	uint32_t		_update_count;
	sharaku_usec_t		_update_start_time;
	sharaku_usec_t		_update_time;
	sharaku_usec_t		_update_cycle_start;
	sharaku_usec_t		_time_start;
	float			_interval;

	uint32_t		_interval_ms;	// jobの周期
	uint32_t		_is_stop:1;
	list_head		_update_list;
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_MODULES_MOD_UPDATE_H_
