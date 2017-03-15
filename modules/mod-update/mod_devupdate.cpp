/* --
 *
 * MIT License
 * 
 * Copyright (c) 2014-2016 Abe Takafumi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <math.h>
#include <stdio.h>
#include <sharaku/types.h>
#include <sharaku/prof.h>
#include <sharaku/log.h>
#include <sharaku/linux/list.h>
#include <sharaku/debug.h>
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
mod_devupdate::mod_devupdate() {
	_devindex = 0;

	sharaku_prof_init(&_prof_interval, "mod_devupdate::interval");
	sharaku_prof_init(&_prof_pre_update_retry, "mod_devupdate::pre_update::retry");
	sharaku_prof_init(&_prof_pre_update_process, "mod_devupdate::pre_update::process");
	sharaku_prof_init(&_prof_update_retry, "mod_devupdate::update::retry");
	sharaku_prof_init(&_prof_update_process, "mod_devupdate::update::process");
	sharaku_prof_init(&_prof_post_update_retry, "mod_devupdate::post_update::retry");
	sharaku_prof_init(&_prof_post_update_process, "mod_devupdate::post_update::process");
	debug_code(sharaku_prof_regist(&_prof_interval));
	debug_code(sharaku_prof_regist(&_prof_pre_update_retry));
	debug_code(sharaku_prof_regist(&_prof_pre_update_process));
	debug_code(sharaku_prof_regist(&_prof_update_retry));
	debug_code(sharaku_prof_regist(&_prof_update_process));
	debug_code(sharaku_prof_regist(&_prof_post_update_retry));
	debug_code(sharaku_prof_regist(&_prof_post_update_process));
}

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
