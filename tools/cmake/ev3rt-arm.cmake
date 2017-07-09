# ----------------------------------------------------------------------------
#
#  MIT License
#  
#  Copyright (c) 2016 Abe Takafumi
#  
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#  
#  The above copyright notice and this permission notice shall be included in all
#  copies or substantial portions of the Software.
#  
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE. *
#
#
# platfoem: ev3rt(toppers)
# arch    : arm
#
# ----------------------------------------------------------------------------

# ------------------------------------------------------------------------------------
# ���ݒ�
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

# ----------------------------------------------------------------------
# gcc�ÓI���
# ----------------------------------------------------------------------

# �֐���aggregate(�z���\���̂Ȃ�?)��Ԃ��ꍇ�Ɍx�����o���B
set(CROSS_FLAGS "${CROSS_FLAGS} -Waggregate-return")

# # �֐��Ăяo�����}�b�`���Ȃ��^�ɃL���X�g����Ă���ꍇ�Ɍx�����o���B
# # C/ObjC
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wbad-function-cast")

# type qualifier(const,volatile�Ȃ�)���O���悤�ȃ|�C���^�̃L���X�g�Ɍx�����o���B
set(CROSS_FLAGS "${CROSS_FLAGS} -Wcast-qual")

# # �Öٌ^�ϊ��̂����A�\���l���ς��\���̂�����̂Ɍx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wconversion")

# �S�ẴR���X�g���N�^�E�f�X�g���N�^��private�ł���A����friend��public static�֐��������Ȃ��N���X(=�g�p�ł��Ȃ��N���X)�ɑ΂��Čx�����o���B
set(CROSS_FLAGS "${CROSS_FLAGS} -Wctor-dtor-privacy")

# __TIME__,__DATE__,__TIMESTAMP__�}�N�����g�p���Ă���ꍇ�Ɍx�����o���B
set(CROSS_FLAGS "${CROSS_FLAGS} -Wdate-time")

# ���z�֐��������Ă���̂Ƀf�X�g���N�^�����z�֐��łȂ��N���X�ɑ΂��āAdelete���g���Ă���ꍇ�Ɍx�����o���B
set(CROSS_FLAGS "${CROSS_FLAGS} -Wdelete-non-virtual-dtor")

# �R�[�h�����������蕡�G�������肵�āA�R���p�C�����œK�������s�ł��Ȃ��ꍇ�Ɍx�����o���B
set(CROSS_FLAGS "${CROSS_FLAGS} -Wdisabled-optimization")

# # float�^���Öق�double�ɃL���X�g����Ă���ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wdouble-promotion")
# # Scott Meyers �� Effective C++ �ɂ�鎟�̕��j�ɉ���Ȃ��L�q�Ɍx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Weffc++")
# # ���������_����==��!=�Ŕ�r���Ă���ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wfloat-equal")
# # -Wuninitialized���w�肳��Ă���ꍇ�ɁA����������Ă��Ȃ��ϐ������ꎩ�g�ŏ��������Ă���ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Winit-self")
# # inline�w�肳��Ă���֐����A�R���p�C����(�֐�����������Ȃǂ̗��R��)�C�����C���W�J���Ȃ������ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Winline")
# # goto��switch�ŕϐ��錾��ʂ�߂���ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wjump-misses-init")
# # �_�����Z�q�̊Ԉ���Ă��邩������Ȃ��g�p�ɑ΂��Čx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wlogical-op")
# # #include�Ŏw�肳�ꂽ�f�B���N�g����������Ȃ��ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wmissing-include-dirs")
# # �����̕������܂ޕ������e�����ɑ΂��Čx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wmultichar")
# # C�ɂ����āA�K�i�Œ�߂�ꂽ�����񒷂́u�ŏ����̍ő咷�v�𒴂��镶����ɑ΂��Čx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Woverlength-strings")
# # �h���N���X�̊֐��Ƃ̖��O���ɂ���āA���N���X��virtual�֐����g���Ȃ��Ȃ�ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Woverloaded-virtual")
# # �֐��^�Avoid�^�ɑ΂���sizeof�̓K�p�ɑ΂��Čx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wpointer-arith")
# # �R���X�g���N�^�̃����o�������q�ƁA�����o�ϐ��̐錾�̏��Ԃ��قȂ�ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wreorder")
# # �I�[�o�[���[�h�ɂ����unsigned ~��enum���Asigned ~�Ɍ^�ϊ������ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wsign-promo")
# # -fstack-protector���w�肳��Ă���ꍇ�ɁA�X�^�b�N�ی삪�Ȃ���Ȃ������֐������݂���ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wstack-protector")
# # switch����default���x���̕��������Ȃ��ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wswitch-default")
# # switch���̑ΏۂɂȂ�l��enum�ŁA���x����enum�̂��ׂĂ̒l�ɂ͑Ή����Ă��Ȃ��ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wswitch-enum")
# # ���[�v�J�E���^���I�[�o�[�t���[����\���������āA���[�v���œK���ł��Ȃ��ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wunsafe-loop-optimizations")
# # �ڔ����̂��Ă��Ȃ����������_�����e�����ɑ΂��Čx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wunsuffixed-float-constants")
#
# # �ϐ����A���̕ϐ����g�̌^�ɃL���X�g���Ă���(���Ӗ��ł���)�ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wuseless-cast")

# �����񃊃e����(const char*)��char*�֌^�ϊ�����ꍇ�Ɍx�����o���B
set(CROSS_FLAGS "${CROSS_FLAGS} -Wwrite-strings")

# # �������e����'0'���k���|�C���^�������萔�Ƃ��Ďg���Ă���ꍇ�Ɍx�����o�͂���B
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wzero-as-null-pointer-constant")

# ----------------------------------------------------------------------

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
