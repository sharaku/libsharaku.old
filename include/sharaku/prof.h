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

#ifndef SHARAKU_PROF_H_
#define SHARAKU_PROF_H_

#include <sharaku/utime.h>
#include <sharaku/linux/list.h>

#ifndef SHARAKU_PROF_IDMAX
#define SHARAKU_PROF_IDMAX 256
#endif

#ifndef SHARAKU_PROF_FILENAME
#define SHARAKU_PROF_FILENAME "./profile.csv"
#endif

#ifndef SHARAKU_PROF_FLASH_INTERVALE
#define SHARAKU_PROF_FLASH_INTERVALE 1000
#endif

typedef struct sharaku_prof {
	struct list_head	list;
	const char		*name;
	uint32_t		count;
	sharaku_usec_t		usec;
	sharaku_usec_t		min;
	sharaku_usec_t		max;
} sharaku_prof_t;

#if defined(SHARAKU_PROF_ENABLE)

#if defined(__cplusplus)
extern "C" {
#endif

#define SHARAKU_PROF_INIT(PROF, NAME)	\
	{					\
		INIT_LIST_HEAD(&(PROF)->list),	\
		NAME,				\
		0,				\
		0,				\
		0,				\
		0				\
	}

static inline void
sharaku_prof_init(sharaku_prof_t *prof, const char *name)
{
	INIT_LIST_HEAD(&prof->list);
	prof->name = name;
	prof->count = 0;
	prof->usec = 0;
	prof->min = 0;
	prof->max = 0;
}

extern void sharaku_prof_regist(sharaku_prof_t *prof);
static inline void
sharaku_prof_add(sharaku_prof_t *prof, sharaku_usec_t us_start, sharaku_usec_t us_end)
{
	sharaku_usec_t	t = us_end - us_start;
	prof->count ++;
	prof->usec += t;
	if (prof->min > t) { prof->min = t;}
	if (prof->max < t) { prof->max = t;}
}
static inline void
sharaku_prof_add_notime(sharaku_prof_t *prof)
{
	prof->count ++;
}
static inline uint32_t
sharaku_prof_get_count(sharaku_prof_t *prof)
{
	return prof->count;
}
static inline sharaku_usec_t
sharaku_prof_get_usec(sharaku_prof_t *prof)
{
	return prof->usec;
}


extern void sharaku_init_prof(void);
extern void sharaku_finl_prof(void);

#if defined(__cplusplus)
}
#endif

#else /* SHARAKU_PROF_ENABLE */

#define sharaku_prof_init(...)
#define sharaku_prof_regist(...)
#define sharaku_prof_add(...)
#define sharaku_prof_add_notime(...)
#define sharaku_prof_get_count(...) 0
#define sharaku_prof_get_usec(...) 0
#define sharaku_init_prof(...)
#define sharaku_finl_prof(...)

#endif /* SHARAKU_PROF_ENABLE */

#endif // SHARAKU_PROF_H_
