# ----------------------------------------------------------------------------
#
#  Copyright Abe Takafumi All Rights Reserved, 2016
#  Author Abe Takafumi
#
#  Modules CMake file for libsharaku
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5)

set(SHARAKU_INCLUDE_PATH
	"${CMAKE_SOURCE_DIR}/platform/${TARGET_OS}/${TARGET_ARCH}/${TARGET_PLATFORM}/include"
	"${CMAKE_SOURCE_DIR}/platform/${TARGET_OS}/${TARGET_ARCH}/${TARGET_PLATFORM}"
	"${CMAKE_SOURCE_DIR}/platform/${TARGET_OS}/${TARGET_ARCH}/include"
	"${CMAKE_SOURCE_DIR}/platform/${TARGET_OS}/${TARGET_ARCH}"
	"${CMAKE_SOURCE_DIR}/platform/${TARGET_OS}/include"
	"${CMAKE_SOURCE_DIR}/platform/${TARGET_OS}"
	"${CMAKE_SOURCE_DIR}/include"
	)
set(SHARAKU_LIB_PATH
	"${CMAKE_SOURCE_DIR}/devices"
	"${CMAKE_SOURCE_DIR}/modules"
	"${CMAKE_SOURCE_DIR}/system"
	)

add_subdirectory(system)
add_subdirectory(devices)
add_subdirectory(modules)

# ’Ç‰Áƒrƒ‹ƒh
if (${TARGET_PLATFORM} STREQUAL "ev3dev")
	add_subdirectory(tools/ev3dev-et.py)
endif()
