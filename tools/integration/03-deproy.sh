#!/bin/sh

readonly DEPLOYBASE_PATH=deproy/
readonly LIBSHARAKU_PATH=${DEPLOYBASE_PATH}libsharaku/
readonly LIBSHARAKU_INCLUDE_PATH=${LIBSHARAKU_PATH}usr/local/include/libsharaku/
readonly LIBSHARAKU_LOCAL_LIB_PATH=${LIBSHARAKU_PATH}usr/local/lib/libsharaku/
readonly LIBSHARAKU_ITRONARM_INCLUDE_PATH=${LIBSHARAKU_INCLUDE_PATH}arm-itron/
readonly LIBSHARAKU_ITRONARM_LIB_PATH=${LIBSHARAKU_LOCAL_LIB_PATH}arm-itron/
readonly LIBSHARAKU_LINUXARM_INCLUDE_PATH=${LIBSHARAKU_INCLUDE_PATH}arm-linux/
readonly LIBSHARAKU_LINUXARM_LIB_PATH=${LIBSHARAKU_LOCAL_LIB_PATH}arm-linux/
readonly LIBSHARAKU_LINUXx86_INCLUDE_PATH=${LIBSHARAKU_INCLUDE_PATH}x86-linux/
readonly LIBSHARAKU_LINUXx86_LIB_PATH=${LIBSHARAKU_LOCAL_LIB_PATH}x86-linux/

# deploy先をクリーンにする
rm -rf ${DEPLOYBASE_PATH}
mkdir -p ${LIBSHARAKU_ITRONARM_INCLUDE_PATH}
mkdir -p ${LIBSHARAKU_ITRONARM_LIB_PATH}
mkdir -p ${LIBSHARAKU_LINUXARM_INCLUDE_PATH}
mkdir -p ${LIBSHARAKU_LINUXARM_LIB_PATH}
mkdir -p ${LIBSHARAKU_LINUXx86_INCLUDE_PATH}
mkdir -p ${LIBSHARAKU_LINUXx86_LIB_PATH}

# ---------------------------------------------------------------------------
# libsharaku-ev3devパッケージのdeployを行う
# こちらは、ev3devに対してインストールする
# ---------------------------------------------------------------------------
readonly LIBSHARAKU_EV3DEV_PATH=${DEPLOYBASE_PATH}libsharaku-ev3dev/
readonly LIBSHARAKU_EV3DEV_LIB_PATH=${LIBSHARAKU_EV3DEV_PATH}usr/local/lib/libsharaku/
mkdir -p ${LIBSHARAKU_EV3DEV_LIB_PATH}
mkdir -p ${LIBSHARAKU_EV3DEV_PATH}usr/lib/
cp -p build/tools/ev3dev-et.py/libEV3Way.so ${LIBSHARAKU_EV3DEV_LIB_PATH}
cp -p build/tools/ev3dev-et.py/libHackEV.so ${LIBSHARAKU_EV3DEV_LIB_PATH}

ln -s /usr/local/lib/libsharaku/libEV3Way.so ${LIBSHARAKU_EV3DEV_PATH}usr/lib/libEV3Way.so
ln -s /usr/local/lib/libsharaku/libHackEV.so ${LIBSHARAKU_EV3DEV_PATH}usr/lib/libHackEV.so

# ---------------------------------------------------------------------------
# libsharakuパッケージのdeploy
# 共通includeのデプロイ後、プラットフォーム固有ファイルのデプロイをする。
# ---------------------------------------------------------------------------
cp -pR ../../include/* ${LIBSHARAKU_ITRONARM_INCLUDE_PATH}
cp -pR ../../include/* ${LIBSHARAKU_LINUXARM_INCLUDE_PATH}
cp -pR ../../include/* ${LIBSHARAKU_LINUXx86_INCLUDE_PATH}

# linux.arm
cp -pR ../../platform/itron/include/* ${LIBSHARAKU_ITRONARM_INCLUDE_PATH}
cp -p  build/devices/libsharaku-dev.linux.arm.ev3dev.a ${LIBSHARAKU_LINUXARM_LIB_PATH}
cp -p  build/modules/libsharaku-mod.linux.arm.ev3dev.a ${LIBSHARAKU_LINUXARM_LIB_PATH}
cp -p  build/system/libsharaku-sys.linux.arm.ev3dev.a ${LIBSHARAKU_LINUXARM_LIB_PATH}

# ev3rt.arm
cp -pR ../../platform/linux/include/* ${LIBSHARAKU_LINUXARM_INCLUDE_PATH}
cp -p  build/devices/libsharaku-dev.ev3rt.arm.a ${LIBSHARAKU_ITRONARM_LIB_PATH}
cp -p  build/modules/libsharaku-mod.ev3rt.arm.a ${LIBSHARAKU_ITRONARM_LIB_PATH}
cp -p  build/system/libsharaku-sys.ev3rt.arm.a ${LIBSHARAKU_ITRONARM_LIB_PATH}

# linux.x86
cp -pR ../../platform/linux/include/* ${LIBSHARAKU_LINUXx86_INCLUDE_PATH}
cp -p  build/devices/libsharaku-dev.linux.x86.a ${LIBSHARAKU_LINUXx86_LIB_PATH}
cp -p  build/modules/libsharaku-mod.linux.x86.a ${LIBSHARAKU_LINUXx86_LIB_PATH}
cp -p  build/system/libsharaku-sys.linux.x86.a ${LIBSHARAKU_LINUXx86_LIB_PATH}

