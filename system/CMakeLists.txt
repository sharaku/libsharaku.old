# ----------------------------------------------------------------------------
#
#  Copyright Abe Takafumi All Rights Reserved, 2016
#  Author Abe Takafumi
#
#  System CMake file for libsharaku
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5)

# ---------------------------------------------------------------
include_directories(${SHARAKU_INCLUDE_PATH})

set(MODULE_SYSTEM
	"sched/task.c"
	"sched/sched.c"
	"debug/log.c"
	"debug/prof.c"
	"activity.c"
	"module.c"
	)

add_library(sharaku-sys.${TARGET_SUFFIX} STATIC
	${MODULE_SYSTEM}
	)
