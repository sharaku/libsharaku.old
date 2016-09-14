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

//-----------------------------------------------------------------------------

class update_group
 : public update_operations
{
 public:
	update_group() {
		INIT_LIST_HEAD(&_update_list);
	}
	// Updateの登録
	void register_update(update_operations* update);

	virtual void	pre_update(const float &interval);
	virtual void	update(const float &interval);
	virtual void	post_update(const float &interval);

 private:
	list_head		_update_list;
};

// mdsetup addのようなもので登録する。
// 登録したものに対してUpdateイベントを発行する、
// 登録時に優先度を設定することで、uodate優先度を設定する。

// 登録されたモジュールにUpdate通知を発行する
// また、定期的に出力情報をポーリングする
class mod_update
 : public update_group
{
 public:
	typedef void (*vehicle_cycle_function)(float interval);
 public:
	mod_update(uint32_t interval);

	// Updateの開始と停止
	void start();
	void stop();

 protected:
	static void mod_update_cycle(struct sharaku_job* job);

 private:
	mod_update();

 private:
	sharaku_usec_t		_time;		// 前回実行時の時間

	uint32_t		_update_count;
	sharaku_usec_t		_update_start_time;
	sharaku_usec_t		_update_time;

	struct sharaku_job	_job_update;
	uint32_t		_interval_ms;	// jobの周期
	uint32_t		_is_stop:1;
	list_head		_cycle_function;
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_MODULES_MOD_UPDATE_H_
