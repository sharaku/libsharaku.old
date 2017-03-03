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
 * SOFTWARE. *
 *
 */

#ifndef SHARAKU_MODULES_MOD_DEVUPDATE_H_
#define SHARAKU_MODULES_MOD_DEVUPDATE_H_

#include <stdint.h>
#include <sharaku/sched.h>
#include <sharaku/list.h>
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
