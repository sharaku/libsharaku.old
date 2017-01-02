#!/bin/sh

readonly VERSIONS=0.0.0
readonly LIBSHARAKU_PACKAGE=libsharaku-${VERSIONS}
readonly LIBSHARAKU_EV3DEV_PACKAGE=libsharaku-ev3dev-${VERSIONS}
readonly BASE_PATH=`pwd`/deproy/

rm -f ${LIBSHARAKU_PACKAGE}.tgz
rm -f ${LIBSHARAKU_EV3DEV_PACKAGE}.tgz

cd ${BASE_PATH}libsharaku
tar zcvf ../${LIBSHARAKU_PACKAGE}.tgz .

cd ${BASE_PATH}libsharaku-ev3dev
tar zcvf ../${LIBSHARAKU_EV3DEV_PACKAGE}.tgz .

