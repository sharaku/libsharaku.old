/* --
 *
 * MIT License
 * 
 * Copyright (c) 2014 Abe Takafumi
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

#if defined(SHARAKU_PROF_ENABLE)

/******************************************************************************
include section
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sharaku/sched.h>
#include <sharaku/prof.h>

/******************************************************************************
struct section
******************************************************************************/

/******************************************************************************
extern values section
******************************************************************************/

/******************************************************************************
static values section
******************************************************************************/
static FILE			*__prof_file;
static struct list_head		__prof_list = LIST_HEAD_INIT(__prof_list);
#if (SHARAKU_PROF_FLASH_INTERVALE == 0)
static struct sharaku_job	__prof_job;
#endif

/******************************************************************************
function
******************************************************************************/
void
sharaku_prof_regist(sharaku_prof_t *prof)
{
	list_add(&prof->list, &__prof_list);
}

static void
__sharaku_prof_flash(void)
{
	sharaku_prof_t *prof;

	fseek(__prof_file, 0, SEEK_SET);
	list_for_each_entry(prof, &__prof_list, sharaku_prof_t, list) {
		fprintf(__prof_file, "%s,%u,%u,%u,%u,%u\n",
			prof->name,
			(unsigned int)prof->count,
			(unsigned int)prof->usec,
			(unsigned int)((prof->count)
					? prof->usec / prof->count : 0),
			(unsigned int)prof->min,
			(unsigned int)prof->max);

#if defined(SHARAKU_PROF_CLEAR_ENABLE)
		// 出力するごとにクリアする場合はこのコードを有効にする
		prof->count	= 0;
		prof->usec	= 0;
		prof->min	= (sharaku_usec_t)0xffffffff;
		prof->max	= 0;
#endif
	}
}

#if (SHARAKU_PROF_FLASH_INTERVALE == 0)
static void
__sharaku_prof_flashjob(struct sharaku_job *job)
{
	__sharaku_prof_flash();
	sharaku_timer_message(job, SHARAKU_PROF_FLASH_INTERVALE,
			      __sharaku_prof_flashjob);
}
#endif

void sharaku_init_prof(void)
{
	__prof_file = fopen(SHARAKU_PROF_FILENAME, "w+");
#if (SHARAKU_PROF_FLASH_INTERVALE == 0)
	sharaku_init_job(&__prof_job);
	sharaku_timer_message(&__prof_job, SHARAKU_PROF_FLASH_INTERVALE,
			       __sharaku_prof_flashjob);
#endif
}

void sharaku_finl_prof(void)
{
	__sharaku_prof_flash();
	fclose(__prof_file);
}

#endif
