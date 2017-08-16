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

#ifndef SHARAKU_MODULES_MOD_UPDATE_H_
#define SHARAKU_MODULES_MOD_UPDATE_H_

#include <stdint.h>
#include <sharaku/sched.h>
#include <sharaku/linux/list.h>
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
	uint32_t get_prof_count_interval();
	sharaku_usec_t get_prof_usec_interval();
	uint32_t get_prof_count_processing();
	sharaku_usec_t get_prof_usec_processing();

 protected:
	static void mod_update_cycle(job_t* job);
	static void mod_pre_update_begin(job_t* job);
	static void mod_pre_update(job_t* job);
	static void mod_pre_update_retry(job_t* job);
	static void mod_pre_update_end(job_t* job);
	static void mod_update_begin(job_t* job);
	static void mod_update_(job_t* job);
	static void mod_update_retry(job_t* job);
	static void mod_update_end(job_t* job);
	static void mod_post_update_begin(job_t* job);
	static void mod_post_update(job_t* job);
	static void mod_post_update_retry(job_t* job);
	static void mod_post_update_end(job_t* job);

 private:
	mod_update();

 private:
	sharaku_usec_t		_time;		// 前回実行時の時間

	job_t			_job_update;
	job_t			_job_update_interval;
	job_t			*_job_update_i;
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
