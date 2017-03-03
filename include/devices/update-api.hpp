/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_DEVICE_UPDATE_API_H_
#define SHARAKU_DEVICE_UPDATE_API_H_

#include <stdint.h>
#include <sharaku/types.h>
#include <sharaku/linux/list.h>
#include <sharaku/prof.h>


NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------
// device_update_operationsインタフェース

class device_update_operations
{
 public:
	virtual void	start_commit(void) = 0;
	virtual void	start_update(void) = 0;
	virtual int32_t	is_ioend(void) = 0;
};

//-----------------------------------------------------------------------------
// updateインタフェース

class update_operations
{
 public:
	update_operations() {
		INIT_LIST_HEAD(&update_list);
	}
	virtual int32_t	pre_update(const float &interval, uint32_t retry_cnt) { return 0; }
	virtual int32_t	update(const float &interval, uint32_t retry_cnt) { return 0; }
	virtual int32_t	post_update(const float &interval, uint32_t retry_cnt) { return 0; }
 public:
	list_head	update_list;

	// profile採取用。
	// 登録は自前で行う。
	sharaku_usec_t		_prof_time_per_update_start;
	sharaku_usec_t		_prof_time_update_start;
	sharaku_usec_t		_prof_time_post_update_start;
	sharaku_prof_t		_prof_interval;			// 全体のインターバル
	sharaku_prof_t		_prof_pre_update_retry;		// pre_updateのリトライ数
	sharaku_prof_t		_prof_pre_update_process;	// pre_updateの処理時間
	sharaku_prof_t		_prof_update_retry;		// updateのリトライ数
	sharaku_prof_t		_prof_update_process;		// updateの処理時間
	sharaku_prof_t		_prof_post_update_retry;	// post_updateのリトライ数
	sharaku_prof_t		_prof_post_update_process;	// post_updateの処理時間
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_DEVICE_UPDATE_API_H_
