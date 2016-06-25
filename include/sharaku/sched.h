/*****************************************************************************/
/*!
@author	abe takafumi
@par	Copyright
2004-2014 abe takafumi All Rights Reserved.
@file	scheduler.h
@brief	プラットフォーム固有スケジューラ機能
	対応プラットフォーム：Windows (C++), Linux (C/C++) ...
******************************************************************************/
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

#if defined(__cplusplus)
}
#endif


#endif /* _SHARAKU_SCHED_H_ */
