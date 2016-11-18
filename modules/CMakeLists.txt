# ----------------------------------------------------------------------------
#
#  Copyright Abe Takafumi All Rights Reserved, 2016
#  Author Abe Takafumi
#
#  Modules CMake file for libsharaku
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5)

# ---------------------------------------------------------------
include_directories(${SHARAKU_INCLUDE_PATH})

set(MODULE_UPDATE
	"mod-update/mod_devupdate.cpp"
	"mod-update/mod-update.cpp"
	)

add_library(sharaku-mod.${TARGET_SUFFIX} STATIC
	${MODULE_UPDATE}
	)
