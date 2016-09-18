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
	virtual int32_t	is_commited(void) = 0;
	virtual int32_t	is_updated(void) = 0;
};

//-----------------------------------------------------------------------------
// updateインタフェース

class update_operations
{
 public:
	update_operations() {
		INIT_LIST_HEAD(&update_list);
	}
	virtual void	pre_update(const float &interval) {}
	virtual void	update(const float &interval) {}
	virtual void	post_update(const float &interval) {}
 public:
	list_head	update_list;
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_DEVICE_UPDATE_API_H_
