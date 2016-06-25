/*****************************************************************************/
/*!
@author	abe takafumi
@par	Copyright
2008-2014 abe takafumi All Rights Reserved.
@file	module.h
@brief	sharakuのmodule機能
	対応プラットフォーム：Windows (C++), Linux (C/C++) ...
******************************************************************************/
#ifndef _SHARAKU_MODULES_H_
#define _SHARAKU_MODULES_H_

/******************************************************************************
include section
******************************************************************************/
#include <stdint.h>
#include <sharaku/task.h>
#include <sharaku/list.h>

////////////////////////////////////////////////////////////////////////
// module IDs
enum MODULE_PRIORITY {
	SHARAKU_RESERVE_MODULE_BEGIN	= 0x80,
	SHARAKU_USER_MODULE_BEGIN	= 0x100,
	SHARAKU_USER_MODULE_END		= 0x3FF,
};


////////////////////////////////////////////////////////////////////////


typedef void(*sharaku_module_stagefunc_t)(void);

struct sharaku_module {
	uint16_t			priority;
	uint16_t			interval;
	sharaku_module_stagefunc_t	init;
	sharaku_module_stagefunc_t	cycle;
	sharaku_module_stagefunc_t	final;
	struct sharaku_job		job;
	struct list_head		list;
};

#if defined(__cplusplus)
extern "C" {
#endif

extern void sharaku_module_regist(struct sharaku_module *module);
extern void sharaku_module_initialize();
extern void sharaku_module_finalize();

#if defined(__cplusplus)
}
#endif

#if defined(__cplusplus)
////////////////////////////////////////////////////////////////////////
// C++言語
class sharaku_module_regist_entry {
public:
	sharaku_module_regist_entry(void (*func)(void)) {
		func();
	}
};

#define SHARAKU_REGIST_MODULE_BEGIN_BASE(ID)	\
	static void sharaku_module_regist_entry##ID(void);	\
	sharaku_module_regist_entry	\
		static_sharaku_module_regist_entry##ID(sharaku_module_regist_entry##ID); \
	static void sharaku_module_regist_entry##ID(void) {

#define SHARAKU_REGIST_MODULE_END_BASE	\
	}

////////////////////////////////////////////////////////////////////////
#elif defined(__GNUC__)
////////////////////////////////////////////////////////////////////////
// gccコンパイラ
#define SHARAKU_REGIST_MODULE_BEGIN_BASE(ID)	\
	static void __attribute__((constructor)) sharaku_module_regist_entry##ID(void) {

#define SHARAKU_REGIST_MODULE_END_BASE	\
	}
////////////////////////////////////////////////////////////////////////
#endif



#define SHARAKU_REGIST_MODULE_BEGIN(ID)					\
	SHARAKU_REGIST_MODULE_BEGIN_BASE(ID)				\
		const uint16_t			priority = ID;		\
		uint16_t			interval = 0;		\
		sharaku_module_stagefunc_t	init = NULL;		\
		sharaku_module_stagefunc_t	cycle = NULL;		\
		sharaku_module_stagefunc_t	final = NULL;		\
		static struct sharaku_module module;

#define SHARAKU_REGIST_MODULE_END					\
		module.priority	= priority;				\
		module.interval	= interval;				\
		module.init	= init;					\
		module.cycle	= cycle;				\
		module.final	= final;				\
		sharaku_init_job(&module.job);				\
		INIT_LIST_HEAD(&(module.list));				\
		sharaku_module_regist(&module);				\
	SHARAKU_REGIST_MODULE_END_BASE

#endif /* _SHARAKU_MODULES_H_ */
