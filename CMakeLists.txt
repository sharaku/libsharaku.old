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


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CROSS_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CROSS_FLAGS} -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${CROSS_FLAGS} -g -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CROSS_FLAGS} -g -O0")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} ${CROSS_FLAGS} -g -O2")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${CROSS_FLAGS} -g -O2")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${CROSS_FLAGS} -O2")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CROSS_FLAGS} -O2")
set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} ${CROSS_FLAGS} -O3")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} ${CROSS_FLAGS} -O3")

add_subdirectory(system)
add_subdirectory(devices)
add_subdirectory(modules)

# 共有ライブラリ用ヘッダーファイルのインストール指定
install(FILES ${CMAKE_SOURCE_DIR}/include/sharaku/vector.hpp DESTINATION include/sharaku)

# 追加ビルド
if (${TARGET_PLATFORM} STREQUAL "ev3dev")
	add_subdirectory(tools/ev3dev-et.py)
endif()
