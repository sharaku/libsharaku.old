/*
 * Copyright Abe Takafumi All Rights Reserved, 2004-2016
 * Author Abe Takafumi
 *
 */
/*****************************************************************************/
#ifndef _SHARAKU_SCHED_H_
#define _SHARAKU_SCHED_H_

/******************************************************************************
include section
******************************************************************************/

/******************************************************************************
extern section
******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

extern int sharaku_entry(void);
extern void sharaku_exit_message(int result);

extern int sharaku_get_exit_message(void);
extern int sharaku_is_exit(void);
extern void sharaku_initialize(void);
extern void sharaku_finalize(void);
extern void sharaku_timer_handler(void);

#if defined(__cplusplus)
}
#endif


#endif /* _SHARAKU_SCHED_H_ */
