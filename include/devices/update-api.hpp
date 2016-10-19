/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_DEVICE_UPDATE_API_H_
#define SHARAKU_DEVICE_UPDATE_API_H_

#include <stdint.h>
#include <sharaku/types.h>
#include <sharaku/list.h>


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
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_DEVICE_UPDATE_API_H_
