# ----------------------------------------------------------------------------
#
#  Copyright Abe Takafumi All Rights Reserved, 2016
#  Author Abe Takafumi
#
# platfoem: ev3dev(linux)
# arch    : arm
#
# ----------------------------------------------------------------------------

set(CMAKE_SYSTEM_NAME Generic)
set(toolchain   ""      CACHE FILEPATH     "")


set(CROSS_TOOLCHAIN_PATH /usr/lib/arm-none-eabi)
set(CROSS_FLAGS "-Wall -Wno-unused-function")
set(CROSS_FLAGS "${CROSS_FLAGS} -DSHARAKU_PROF_ENABLE -DSHARAKU_PROF_CLEAR_ENABLE")

set(CMAKE_C_FLAGS ${CROSS_FLAGS})
set(CMAKE_CXX_FLAGS ${CROSS_FLAGS})
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -O2")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2")
set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS_MINSIZEREL "-O3")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-O3")


set(CROSS_COMPILE "/usr/bin/arm-none-eabi-")
set(CMAKE_C_COMPILER   "${CROSS_COMPILE}gcc" "-specs=rdimon.specs")
set(CMAKE_CXX_COMPILER "${CROSS_COMPILE}g++" "-specs=rdimon.specs")
set(CMAKE_ASM_COMPILER "${CROSS_COMPILE}as")
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CROSS_TOOLCHAIN_PATH}/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CROSS_TOOLCHAIN_PATH}/lib)
set(CMAKE_INSTALL_PREFIX ${CROSS_TOOLCHAIN_PATH}/)

set(TARGET_OS itron)
set(TARGET_ARCH arm)
set(TARGET_PLATFORM ev3rt)
set(TARGET_SUFFIX "ev3rt.${TARGET_ARCH}")

