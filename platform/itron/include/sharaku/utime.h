/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */
#ifndef _SHARAKU_UTIME_H
#define _SHARAKU_UTIME_H

// #include <stdint.h>
// #include <kernel.h>
typedef uint32_t	sharaku_usec_t;

static sharaku_usec_t sharaku_get_usec() {
//	SYSTIM	tm;
//	get_tim(&tm);
//	return (sharaku_usec_t)(tm * 1000);
	return 0;
}

#endif /* _SHARAKU_UTIME_H */
