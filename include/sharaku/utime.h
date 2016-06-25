#ifndef _SHARAKU_UTIME_H
#define _SHARAKU_UTIME_H

#include <stdint.h>
typedef uint32_t	sharaku_usec_t;

#if defined(WIN32)
static sharaku_usec_t sharaku_get_usec() {
	return (sharaku_usec_t)(timeGetTime() * 1000);
}
#elif defined(__linux__)
#include <sys/time.h>
static sharaku_usec_t sharaku_get_usec() {
	struct timeval tv;
	sharaku_usec_t  usec;
	gettimeofday(&tv, NULL);
	usec = tv.tv_usec + (tv.tv_sec * 1000 * 1000);
	return usec;
}
#elif defined(__itron__)
#include <kernel.h>
static sharaku_usec_t sharaku_get_usec() {
	SYSTIM	tm;
	get_tim(&tm);
	return (sharaku_usec_t)(tm * 1000);
}
#endif

#endif /* _SHARAKU_UTIME_H */
