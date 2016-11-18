# ----------------------------------------------------------------------------
#
#  Copyright Abe Takafumi All Rights Reserved, 2016
#  Author Abe Takafumi
#
#  System CMake file for libsharaku
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5)

# ---------------------------------------------------------------
include_directories(
	${SHARAKU_INCLUDE_PATH}
	/usr/arm-linux-gnueabi/include
	/usr/arm-linux-gnueabi/include/python2.7
	)
link_directories(${SHARAKU_LIB_PATH})

add_library(EV3Way SHARED 
	EV3Way.cpp
	)
target_link_libraries(
	EV3Way
	sharaku-sys.${TARGET_SUFFIX}
	sharaku-dev.${TARGET_SUFFIX}
	sharaku-mod.${TARGET_SUFFIX}
	pthread
	boost_python
	python2.7
	)

add_library(HackEV SHARED 
	HackEV.cpp
	)
target_link_libraries(
	HackEV
	sharaku-sys.${TARGET_SUFFIX}
	sharaku-dev.${TARGET_SUFFIX}
	sharaku-mod.${TARGET_SUFFIX}
	pthread
	boost_python
	python2.7
	)
