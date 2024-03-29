﻿/* --
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
 * SOFTWARE. *
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
