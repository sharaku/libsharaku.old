# ----------------------------------------------------------------------------
#
#  Copyright Abe Takafumi All Rights Reserved, 2016
#  Author Abe Takafumi
#
# platfoem: ev3dev(linux)
# arch    : arm
#
# ----------------------------------------------------------------------------

set(CMAKE_SYSTEM_NAME Linux)

set(CROSS_PREFIX arm-linux-gnueabi-)
set(CROSS_TOOLCHAIN_PATH /usr/arm-linux-gnueabi)
set(CROSS_FLAGS "-Wall -Wno-unused-function")
set(CROSS_FLAGS "${CROSS_FLAGS} -DSHARAKU_PROF_ENABLE -DSHARAKU_PROF_CLEAR_ENABLE")

set(CMAKE_C_COMPILER ${CROSS_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${CROSS_PREFIX}g++)
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CROSS_TOOLCHAIN_PATH}/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CROSS_TOOLCHAIN_PATH}/lib)
set(CMAKE_INSTALL_PREFIX ${CROSS_TOOLCHAIN_PATH}/)


set(TARGET_OS linux)
set(TARGET_ARCH arm)
set(TARGET_PLATFORM ev3dev)
set(TARGET_SUFFIX "${TARGET_OS}.${TARGET_ARCH}.${TARGET_PLATFORM}")
