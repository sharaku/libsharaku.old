/* --
 *
 * MIT License
 * 
 * Copyright (c) 2004-2017 Abe Takafumi
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

// 
//
//  +---------+               sharaku_async_message             +-------+
//  |  READY  | <---------------------------------------------- |  RUN  |
//  |         | ----------------------------------------------> |       |
//  +---------+                   スケジュール                  +-------+
//   ｜↑ ↑                                                      ｜｜
//   ｜｜ ｜                                                      ｜｜
//   ｜｜ ｜                                                      ｜｜
//   ｜｜ ｜                                                      ｜｜
//   ｜｜ ｜    時間経過        +---------+ sharaku_timer_message ｜｜
//   ｜｜  -------------------- |  WAIT   | <---------------------- ｜
//   ｜｜                       |         |                         ｜
//   ｜｜                       +---------+                         ｜
//   ｜｜                          ｜↑                             ｜
//   ｜｜    sharaku_cancel_message｜｜sharaku_timer_message        ｜
//   ｜｜                          ↓｜                             ｜
//   ｜｜ sharaku_async_message +---------+                         ｜
//   ｜ ----------------------- | SUSPEND | <------------------------
//    ------------------------> |         |   job関数終了後、スケジュールなし
//    sharaku_cancel_message    +---------+
//                              ↑ sharaku_init_job
//
//
//       +---------+    +---------+    +---------+    +---------+
//       |   CPU   |    |   CPU   |    |   CPU   |    |   CPU   |
//       +---------+    +---------+    +---------+    +---------+
//           ｜             ｜             ｜             ｜
//           ｜ 実行        ｜ 実行        ｜ 実行        ｜ 実行
//           ↓             ↓             ↓             ↓
//       +---------+    +---------+    +---------+    +---------+
//       |workqueue|    |workqueue|    |workqueue|    |workqueue|
//       +---------+    +---------+    +---------+    +---------+
//          ｜↑           ｜↑           ｜↑           ｜↑
//          ｜｜           ｜｜           ｜｜           ｜｜
//          ｜｜           ｜｜           ｜｜           ｜｜
//          ｜ ---------------------------------------------
//          ｜             ｜        ｜   ｜             ｜
//           ------------------------｜-------------------
//                                 ｜｜
//                                 ↓｜
//                     +------------------------+
//                     |スケジューラコンテキスト|
//                     | - job list             |
//                     | - timer list           |
//                     +------------------------+


#ifndef _SHARAKU_SCHED_H_
#define _SHARAKU_SCHED_H_

#include <stdint.h>
#include <sharaku/plist.h>
#include <sharaku/thread.h>

struct sharaku_sched_context {
	uint8_t			is_initialize:1;
	uint8_t			is_stop:1;
	uint8_t			rsv:7;
	int64_t			base_time;

	// 優先度付きjobリスト
	// 一般ユーザが使用可能な優先度は 0 ～ 255
	// システムは-99 ～ 0
	struct plist_head	plist;
	// 時間順のjobリスト
	// 起動時の時間を0として、経過msで登録する。
	struct plist_head	timer_plist;
	sharaku_mutex_t		plist_mx;
	sharaku_mutex_t		timer_plist_mx;
};

#define SHARAKU_CONTEXT_INIT(context)	\
	{							\
		0,						\
		0,						\
		0,						\
		0,						\
		PLIST_HEAD_INIT((context).plist),		\
		PLIST_HEAD_INIT((context).timer_plist),		\
	}


struct sharaku_job;
typedef void(*sharaku_job_stagefunc_t)(struct sharaku_job*);

struct sharaku_job {
	struct plist_node	node;		///< スケジューラのノード
	sharaku_job_stagefunc_t	callback;	///< コールバック
	int32_t			prio;		///< 優先度
	int64_t			milli_sec;	///< タイムアウト値
	struct sharaku_sched_context*
				context;	///< スケジューラコンテキスト
	uint8_t			status;		///< 現在の状態
#define SHARAKU_JOB_STATUS_SUSPEND	1
#define SHARAKU_JOB_STATUS_READY	2
#define SHARAKU_JOB_STATUS_RUN		3
#define SHARAKU_JOB_STATUS_WAIT		4

	uint8_t			is_timeout:1;	///< タイムアウトしているか
	uint8_t			rsv_flg:7;	///< 
	uint32_t		magic;		// マジックコード
#define SHARAKU_TASK_MAGIC	0xF58AABE3
};

#define SHARAKU_DEFAULT_PRIO 128

#if defined(__cplusplus)
extern "C" {
#endif

// スケジューラの初期化
extern void sharaku_init_sched(void);
// スケジューラの破棄
extern void sharaku_finl_sched(void);
// スケジューラの処理
extern void sharaku_workqueue(void);

// スケジューラへの登録
extern void sharaku_init_job(struct sharaku_job *job);
extern void sharaku_init_job_prio(struct sharaku_job *job, int prio);
extern void sharaku_async_message(struct sharaku_job *job,
				sharaku_job_stagefunc_t cb);
extern void sharaku_timer_message(struct sharaku_job *job,
				uint32_t ms, sharaku_job_stagefunc_t cb);
extern void sharaku_cancel_message(struct sharaku_job *job);
#if defined(__cplusplus)
}
#endif


#endif /* _SHARAKU_SCHED_H_ */
