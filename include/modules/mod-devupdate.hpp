/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_MODULES_MOD_DEVUPDATE_H_
#define SHARAKU_MODULES_MOD_DEVUPDATE_H_

#include <stdint.h>
#include <sharaku/sched.h>
#include <sharaku/linux/list.h>
#include <devices/update-api.hpp>

NAMESPACE_SHARAKU_BEGIN

#define SHARAKU_MOD_DEVUPDATE_MAX	16
// 登録されたデバイスをupdate, commitする。
// デバイスのキックは一度に行う。
// その後、全デバイスのupdate, commitが完了するまで待ち合わせる。
class mod_devupdate
 : public update_operations
{
 public:
	mod_devupdate();

	operator update_operations*() {
		return (update_operations *)this;
	}

	// Updateの登録
	void register_update(device_update_operations* update);

	virtual int32_t	pre_update(const float &interval, uint32_t retry_cnt);
	virtual int32_t	post_update(const float &interval, uint32_t retry_cnt);

 private:
	device_update_operations	*_dev_list[SHARAKU_MOD_DEVUPDATE_MAX];
	uint32_t			_devindex;
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_MODULES_MOD_UPDATE_H_
