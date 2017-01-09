/*
 * Copyright Abe Takafumi All Rights Reserved, 2017
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_SYSTEM_H_
#define _SHARAKU_SYSTEM_H_

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

extern void sharaku_timer_handler(uint32_t cyc_ms);
extern void sharaku_cpu_handler(void);
extern void sharaku_exit_message(int result);
extern int sharaku_get_exit_message(void);
extern int sharaku_entry(void);

#if defined(__cplusplus)
}
#endif


#endif /* _SHARAKU_SCHED_H_ */
