/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */
#ifndef _SHARAKU_UTIME_H
#define _SHARAKU_UTIME_H

#include <stdint.h>
#include <sys/time.h>
typedef uint32_t	sharaku_usec_t;

static sharaku_usec_t sharaku_get_usec() {
	struct timeval tv;
	sharaku_usec_t  usec;
	gettimeofday(&tv, NULL);
	usec = tv.tv_usec + (tv.tv_sec * 1000 * 1000);
	return usec;
}

#endif /* _SHARAKU_UTIME_H */
