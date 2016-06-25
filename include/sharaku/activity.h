/*****************************************************************************/
/*!
@author	abe takafumi
@par	Copyright
2008-2014 abe takafumi All Rights Reserved.
@file	activity.h
@brief	sharakuのactivity機能
	対応プラットフォーム：Windows (C++), Linux (C/C++) ...
******************************************************************************/
#ifndef _SHARAKU_ACTIVITY_H_
#define _SHARAKU_ACTIVITY_H_

/******************************************************************************
include section
******************************************************************************/
#include <stdint.h>
#include <sharaku/task.h>
#include <sharaku/list.h>


////////////////////////////////////////////////////////////////////////

// Activity遷移の動作
enum SHARAKU_ACTIVITY_TYPE {
	SHARAKU_ACTIVITY_TYPE_ACTIVITY_NON,		///< 指定なし
	SHARAKU_ACTIVITY_TYPE_ACTIVITY_CLEAR_TOP,	///< StackのActivity破棄
	SHARAKU_ACTIVITY_TYPE_ACTIVITY_FINISH,		///< Activityの終了
	SHARAKU_ACTIVITY_TYPE_ACTIVITY_EXIT,		///< 全Activityの終了
};
// Activity遷移機能
enum SHARAKU_ACTIVITY_SWITCH {
	SHARAKU_ACTIVITY_SWITCH_SIMPLE,		///< シンプルな切り替え
};
enum SHARAKU_ACTIVITY_STATUS {
	SHARAKU_ACTIVITY_STATUS_IDLE,
	SHARAKU_ACTIVITY_STATUS_RESERVE,
	SHARAKU_ACTIVITY_STATUS_ACTIVE,
	SHARAKU_ACTIVITY_STATUS_SUSPEND,
	SHARAKU_ACTIVITY_STATUS_STOP,
};
enum SHARAKU_ACTIVITY_MESSAGE {
	SHARAKU_ACTIVITY_MSG_CREATE,
	SHARAKU_ACTIVITY_MSG_START,
	SHARAKU_ACTIVITY_MSG_RESUME,
	SHARAKU_ACTIVITY_MSG_PAUSE,
	SHARAKU_ACTIVITY_MSG_STOP,
	SHARAKU_ACTIVITY_MSG_DESTROY,
	SHARAKU_ACTIVITY_MSG_RESTART,
	SHARAKU_ACTIVITY_MSG_ROUTINE,
	SHARAKU_ACTIVITY_MSG_MAX,
};

typedef void(*sharaku_activity_stagefunc_t)(void);

struct sharaku_activity {
	uint32_t			id;
	uint16_t			interval;
	uint8_t				status;
	uint8_t				current:1;
	uint8_t				rsv:7;
	sharaku_activity_stagefunc_t	message_stage[SHARAKU_ACTIVITY_MSG_MAX];
	struct sharaku_job		job;
	struct list_head		list;
	struct list_head		active_list;
};

#if defined(__cplusplus)
extern "C" {
#endif

extern void sharaku_activity_regist(struct sharaku_activity *activity);
extern void sharaku_activity_initialize(void);
extern void sharaku_activity_finalize(void);
extern void sharaku_activity_start(uint32_t id, uint8_t type, uint8_t sw);
extern void sharaku_activity_finish(void);

#if defined(__cplusplus)
}
#endif


#if defined(__cplusplus)
////////////////////////////////////////////////////////////////////////
// C++言語
class sharaku_activity_regist_entry {
public:
	sharaku_activity_regist_entry(void (*func)(void)) {
		func();
	}
};

#define SHARAKU_REGIST_ACTIVITY_BEGIN_BASE(ID)		\
	static void sharaku_activity_regist_entry##ID(void);	\
	sharaku_activity_regist_entry	\
		static_sharaku_activity_regist_entry##ID(sharaku_activity_regist_entry##ID); \
	static void sharaku_activity_regist_entry##ID(void) {

#define SHARAKU_REGIST_ACTIVITY_END_BASE	\
	}

#elif defined(__GNUC__)
////////////////////////////////////////////////////////////////////////
// gccコンパイラ
#define SHARAKU_REGIST_ACTIVITY_BEGIN_BASE(ID)	\
	static void __attribute__((constructor)) sharaku_activity_regist_entry##ID(void) {

#define SHARAKU_REGIST_ACTIVITY_END_BASE	\
	}
#endif


#define SHARAKU_REGIST_ACTIVITY_BEGIN(ID)				\
	SHARAKU_REGIST_ACTIVITY_BEGIN_BASE(ID)				\
		const uint32_t		activity_id = ID;		\
		uint32_t		interval = 0;			\
		uint8_t			current = 0;			\
		sharaku_activity_stagefunc_t	create = NULL;		\
		sharaku_activity_stagefunc_t	start = NULL;		\
		sharaku_activity_stagefunc_t	resume = NULL;		\
		sharaku_activity_stagefunc_t	pause = NULL;		\
		sharaku_activity_stagefunc_t	stop = NULL;		\
		sharaku_activity_stagefunc_t	destroy = NULL;		\
		sharaku_activity_stagefunc_t	restart = NULL;		\
		sharaku_activity_stagefunc_t	routine = NULL;		\
		static struct sharaku_activity	activity;

#define SHARAKU_REGIST_ACTIVITY_END	\
		activity.id		= activity_id;			\
		activity.current	= current;			\
		activity.interval	= interval;			\
		activity.message_stage[SHARAKU_ACTIVITY_MSG_CREATE] = create; \
		activity.message_stage[SHARAKU_ACTIVITY_MSG_START] = start;  \
		activity.message_stage[SHARAKU_ACTIVITY_MSG_RESUME] = resume; \
		activity.message_stage[SHARAKU_ACTIVITY_MSG_PAUSE] = pause;  \
		activity.message_stage[SHARAKU_ACTIVITY_MSG_STOP] = stop; \
		activity.message_stage[SHARAKU_ACTIVITY_MSG_DESTROY] = destroy; \
		activity.message_stage[SHARAKU_ACTIVITY_MSG_RESTART] = restart; \
		activity.message_stage[SHARAKU_ACTIVITY_MSG_ROUTINE] = routine; \
		activity.status		= SHARAKU_ACTIVITY_STATUS_IDLE;	\
		sharaku_init_job(&activity.job);			\
		INIT_LIST_HEAD(&(activity.list));			\
		INIT_LIST_HEAD(&(activity.active_list));		\
		sharaku_activity_regist(&activity);			\
	SHARAKU_REGIST_ACTIVITY_END_BASE


#endif /* _SHARAKU_ACTIVITY_H_ */
