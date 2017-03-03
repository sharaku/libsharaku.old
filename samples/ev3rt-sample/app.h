/* --
 *
 * MIT License
 * 
 * Copyright (c) 2016-2017 Abe Takafumi
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

// ターゲット依存の定義
#include "target_test.h"

//  各タスクの優先度の定義
#define MAIN_PRIORITY	5	// メインタスクの優先度
				// HIGH_PRIORITYより高くする必要がある

#define HIGH_PRIORITY	9	// 並行実行されるタスクの優先度
#define MID_PRIORITY	10
#define LOW_PRIORITY	11

// ターゲットに依存する可能性のある定数の定義
#ifndef TASK_PORTID
#define	TASK_PORTID		1			/* 文字入力するシリアルポートID */
#endif /* TASK_PORTID */

#ifndef STACK_SIZE
#define	STACK_SIZE		4096		/* タスクのスタックサイズ */
#endif /* STACK_SIZE */

// タスクエントリ関数のプロトタイプ宣言
#ifndef TOPPERS_MACRO_ONLY
extern void cycle_task(intptr_t exinf);
extern void main_task(intptr_t exinf);
extern void timer_task(intptr_t exinf);
#endif /* TOPPERS_MACRO_ONLY */
