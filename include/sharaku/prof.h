/*****************************************************************************/
/*!
@author	abe takafumi
@par	Copyright
2014 abe takafumi All Rights Reserved.
@file	prof.h
@brie	統計情報収集機能
	対応プラットフォーム：Windows (C++), Linux (C/C++) ...
******************************************************************************/
#ifndef SHARAKU_PROF_H_
#define SHARAKU_PROF_H_

#include <sharaku/utime.h>
#include <sharaku/list.h>

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
	char			*name;
	sharaku_usec_t		count;
	sharaku_usec_t		usec;
	sharaku_usec_t		min;
	sharaku_usec_t		max;
} sharaku_prof_t;

#if defined(SHARAKU_PROF_ENABLE)

#if defined(__cplusplus)
extern "C" {
#endif

static void
sharaku_prof_init(sharaku_prof_t *prof, char *name)
{
	INIT_LIST_HEAD(&prof->list);
	prof->name = name;
	prof->count = 0;
	prof->usec = 0;
	prof->min = 0;
	prof->max = 0;
}

extern void sharaku_prof_regist(sharaku_prof_t *prof);
static void inline
sharaku_prof_add(sharaku_prof_t *prof, sharaku_usec_t us_start, sharaku_usec_t us_end)
{
	sharaku_usec_t	t = us_end - us_start;
	prof->count ++;
	prof->usec += t;
	if (prof->min > t) { prof->min = t;}
	if (prof->max < t) { prof->max = t;}
}
static void inline
sharaku_prof_add_notime(sharaku_prof_t *prof)
{
	prof->count ++;
}
static uint32_t inline
sharaku_prof_get_count(sharaku_prof_t *prof)
{
	return prof->count;
}
static sharaku_usec_t inline
sharaku_prof_get_usec(sharaku_prof_t *prof)
{
	return prof->usec;
}


extern void sharaku_prof_initialize(void);
extern void sharaku_prof_finalize(void);

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
#define sharaku_prof_initialize(...)
#define sharaku_prof_finalize(...)

#endif /* SHARAKU_PROF_ENABLE */

#endif // SHARAKU_PROF_H_
