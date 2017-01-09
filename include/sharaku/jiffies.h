/*
 * Copyright Abe Takafumi All Rights Reserved, 2017
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_JIFFIES_H_
#define _SHARAKU_JIFFIES_H_

#include <stdint.h>

/* Parameters used to convert the timespec values: */
#define HZ_PER_MSEC     1L
#define HZ_PER_USEC     1000L

#if defined(__cplusplus)
extern "C" {
#endif

extern uint64_t volatile jiffies;

static inline uint32_t
jiffies_to_msecs(const unsigned long j)
{
	return jiffies / HZ_PER_MSEC;
}

static inline uint32_t
jiffies_to_usecs(const unsigned long j)
{
	return jiffies / HZ_PER_USEC;
}

static inline uint32_t
sharaku_get_msec(void)
{
	return jiffies_to_msecs(jiffies);
}

#if defined(__cplusplus)
}
#endif

#endif /* _SHARAKU_JIFFIES_H_ */
