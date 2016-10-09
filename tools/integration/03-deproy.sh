#!/bin/sh

readonly DEPLOYBASE_PATH=src/

# deploy先をクリーンにする
rm -rf ${DEPLOYBASE_PATH}

# libsharakuパッケージのdeployを行う
readonly LIBSHARAKU_PATH=${DEPLOYBASE_PATH}libsharaku/
readonly LIBSHARAKU_INCLUDE_PATH=${LIBSHARAKU_PATH}usr/local/include/libsharaku/
readonly LIBSHARAKU_LIB_PATH=${LIBSHARAKU_PATH}usr/local/lib/libsharaku/
mkdir -p ${LIBSHARAKU_INCLUDE_PATH}
mkdir -p ${LIBSHARAKU_LIB_PATH}
cp -pR ../../include/* ${LIBSHARAKU_INCLUDE_PATH}
cp -p ../../devices/libsharaku-dev.arm.a ${LIBSHARAKU_LIB_PATH}
cp -p ../../devices/libsharaku-dev.x86.a ${LIBSHARAKU_LIB_PATH}
cp -p ../../modules/libsharaku-mod.arm.a ${LIBSHARAKU_LIB_PATH}
cp -p ../../modules/libsharaku-mod.x86.a ${LIBSHARAKU_LIB_PATH}
cp -p ../../system/libsharaku-sys.arm.a ${LIBSHARAKU_LIB_PATH}
cp -p ../../system/libsharaku-sys.x86.a ${LIBSHARAKU_LIB_PATH}

# libsharaku-ev3devパッケージのdeployを行う
readonly LIBSHARAKU_EV3DEV_PATH=${DEPLOYBASE_PATH}libsharaku-ev3dev/
readonly LIBSHARAKU_EV3DEV_LIB_PATH=${LIBSHARAKU_EV3DEV_PATH}usr/local/lib/libsharaku/
mkdir -p ${LIBSHARAKU_EV3DEV_LIB_PATH}
cp -p ../ev3dev-et.py/EV3Way.so ${LIBSHARAKU_EV3DEV_LIB_PATH}
cp -p ../ev3dev-et.py/HackEV.so ${LIBSHARAKU_EV3DEV_LIB_PATH}

