/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2015
 * Author Abe Takafumi
 *
 */

#include <math.h>
#include <stdio.h>
#include <sharaku/types.h>
#include <sharaku/prof.h>
#include <sharaku/log.h>
#include <sharaku/list.h>
#include <sharaku/module.h>
#include <modules/mod-devupdate.hpp>

NAMESPACE_SHARAKU_BEGIN

/******************************************************************************
definition section
******************************************************************************/

/******************************************************************************
class section
******************************************************************************/

/******************************************************************************
mod_update
******************************************************************************/
void
mod_devupdate::register_update(device_update_operations* update)
{
	_dev_list[_devindex] = update;
	_devindex++;
}

int32_t
mod_devupdate::pre_update(const float &interval, uint32_t retry_cnt)
{
	if (retry_cnt == 0) {
		sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);
		for (int i = 0; i < _devindex; i++) {
			_dev_list[i]->start_update();
		}
		return EAGAIN;
	} else {
		sharaku_db_trace("retry", 0, 0, 0, 0, 0, 0);
		int completeed = 1;
		for (int i = 0; i < _devindex; i++) {
			if (_dev_list[i]->is_ioend() == 0) {
				completeed = 0;
			}
		}
		if (completeed) {
			return 0;
		} else {
			return EAGAIN;
		}
	}
}

int32_t
mod_devupdate::post_update(const float &interval, uint32_t retry_cnt)
{
	if (retry_cnt == 0) {
		sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);
		for (int i = 0; i < _devindex; i++) {
			_dev_list[i]->start_commit();
		}
		return EAGAIN;
	} else {
		sharaku_db_trace("retry", 0, 0, 0, 0, 0, 0);
		int completeed = 1;
		for (int i = 0; i < _devindex; i++) {
			if (_dev_list[i]->is_ioend() == 0) {
				completeed = 0;
			}
		}
		if (completeed) {
			return 0;
		} else {
			return EAGAIN;
		}
	}
}

NAMESPACE_SHARAKU_END
