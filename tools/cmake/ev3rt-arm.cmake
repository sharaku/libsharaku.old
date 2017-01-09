# ----------------------------------------------------------------------------
#
#  Copyright Abe Takafumi All Rights Reserved, 2016
#  Author Abe Takafumi
#
# platfoem: ev3dev(linux)
# arch    : arm
#
# ----------------------------------------------------------------------------

# ------------------------------------------------------------------------------------
# ä¬ã´ê›íË
# ------------------------------------------------------------------------------------
set(TOPPERS_INSTALL_PATH "/srv/ev3rt-beta5-2-release/hrp2")

set(EV3RT_INCLUDE_PATH
	"${TOPPERS_INSTALL_PATH}/include"
	"${TOPPERS_INSTALL_PATH}/arch"
	"${TOPPERS_INSTALL_PATH}/arch/arm_gcc/am1808"
	"${TOPPERS_INSTALL_PATH}/arch/arm_gcc/common"
	"${TOPPERS_INSTALL_PATH}/target/ev3_gcc"
	"${TOPPERS_INSTALL_PATH}/target/ev3_gcc/api/include"
	"${TOPPERS_INSTALL_PATH}/target/ev3_gcc/api/src"
	"${TOPPERS_INSTALL_PATH}/workspace/common/lib/libcpp-ev3/include"
	"${TOPPERS_INSTALL_PATH}/workspace/OBJ"
	)

# ------------------------------------------------------------------------------------
set(CMAKE_SYSTEM_NAME Generic)
set(toolchain   ""      CACHE FILEPATH     "")

set(CROSS_TOOLCHAIN_PATH /usr/lib/arm-none-eabi)
set(CROSS_FLAGS "-Wall -Wno-unused-function -Wunused-variable")
set(CROSS_FLAGS "${CROSS_FLAGS} -Dgcc -mcpu=arm926ej-s -mlittle-endian -DUSE_CFG_PASS3 -g -Wall -O2 -DBUILD_EV3_PLATFORM -DCONFIG_FB_DEFERRED_IO -D__TARGET_ARCH_ARM=5")
set(CROSS_FLAGS "${CROSS_FLAGS} -DSHARAKU_PROF_ENABLE -DSHARAKU_PROF_CLEAR_ENABLE")
set(CROSS_FLAGS "${CROSS_FLAGS} -DSHARAKU_DBLOG_FILENAME=\\\"/sharaku_db_log.log\\\"")
set(CROSS_FLAGS "${CROSS_FLAGS} -DSHARAKU_DATALOGGER_FILENAME=\\\"/sharaku_dev_logger.csv\\\"")
set(CROSS_FLAGS "${CROSS_FLAGS} -DSHARAKU_PROF_FILENAME=\\\"/profile.csv\\\"")
set(CROSS_FLAGS "${CROSS_FLAGS} -D__ev3rt__")

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

include_directories(${EV3RT_INCLUDE_PATH})
