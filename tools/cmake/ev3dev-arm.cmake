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
# platfoem: ev3dev(linux)
# arch    : arm
#
# ----------------------------------------------------------------------------

set(CMAKE_SYSTEM_NAME Linux)

# sharaku�̐ݒ�
set(TARGET_OS linux)
set(TARGET_ARCH arm)
set(TARGET_PLATFORM ev3dev)
set(TARGET_SUFFIX "${TARGET_OS}.${TARGET_ARCH}.${TARGET_PLATFORM}")

# ----------------------------------------------------------------------
# gcc�ÓI���
# ----------------------------------------------------------------------
set(CROSS_FLAGS_C "")
set(CROSS_FLAGS_CXX "")

# �֐���aggregate(�z���\���̂Ȃ�?)��Ԃ��ꍇ�Ɍx�����o���B
set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Waggregate-return")
set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Waggregate-return")

# # �֐��Ăяo�����}�b�`���Ȃ��^�ɃL���X�g����Ă���ꍇ�Ɍx�����o���B
# # C/ObjC
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wbad-function-cast")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wbad-function-cast")

# type qualifier(const,volatile�Ȃ�)���O���悤�ȃ|�C���^�̃L���X�g�Ɍx�����o���B
set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wcast-qual")
set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wcast-qual")

# # �Öٌ^�ϊ��̂����A�\���l���ς��\���̂�����̂Ɍx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wconversion")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wconversion")

# �S�ẴR���X�g���N�^�E�f�X�g���N�^��private�ł���A����friend��public static�֐��������Ȃ��N���X(=�g�p�ł��Ȃ��N���X)�ɑ΂��Čx�����o���B
#set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wctor-dtor-privacy")
set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wctor-dtor-privacy")

# __TIME__,__DATE__,__TIMESTAMP__�}�N�����g�p���Ă���ꍇ�Ɍx�����o���B
set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wdate-time")
set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wdate-time")

# ���z�֐��������Ă���̂Ƀf�X�g���N�^�����z�֐��łȂ��N���X�ɑ΂��āAdelete���g���Ă���ꍇ�Ɍx�����o���B
#set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wdelete-non-virtual-dtor")
set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wdelete-non-virtual-dtor")

# �R�[�h�����������蕡�G�������肵�āA�R���p�C�����œK�������s�ł��Ȃ��ꍇ�Ɍx�����o���B
set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wdisabled-optimization")
set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wdisabled-optimization")

# # float�^���Öق�double�ɃL���X�g����Ă���ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wdouble-promotion")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wdouble-promotion")
# # Scott Meyers �� Effective C++ �ɂ�鎟�̕��j�ɉ���Ȃ��L�q�Ɍx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Weffc++")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Weffc++")
# # ���������_����==��!=�Ŕ�r���Ă���ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wfloat-equal")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wfloat-equal")
# # -Wuninitialized���w�肳��Ă���ꍇ�ɁA����������Ă��Ȃ��ϐ������ꎩ�g�ŏ��������Ă���ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Winit-self")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Winit-self")
# # inline�w�肳��Ă���֐����A�R���p�C����(�֐�����������Ȃǂ̗��R��)�C�����C���W�J���Ȃ������ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Winline")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Winline")
# # goto��switch�ŕϐ��錾��ʂ�߂���ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wjump-misses-init")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wjump-misses-init")
# # �_�����Z�q�̊Ԉ���Ă��邩������Ȃ��g�p�ɑ΂��Čx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wlogical-op")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wlogical-op")
# # #include�Ŏw�肳�ꂽ�f�B���N�g����������Ȃ��ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wmissing-include-dirs")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wmissing-include-dirs")
# # �����̕������܂ޕ������e�����ɑ΂��Čx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wmultichar")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wmultichar")
# # C�ɂ����āA�K�i�Œ�߂�ꂽ�����񒷂́u�ŏ����̍ő咷�v�𒴂��镶����ɑ΂��Čx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Woverlength-strings")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Woverlength-strings")
# # �h���N���X�̊֐��Ƃ̖��O���ɂ���āA���N���X��virtual�֐����g���Ȃ��Ȃ�ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Woverloaded-virtual")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Woverloaded-virtual")
# # �֐��^�Avoid�^�ɑ΂���sizeof�̓K�p�ɑ΂��Čx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wpointer-arith")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wpointer-arith")
# # �R���X�g���N�^�̃����o�������q�ƁA�����o�ϐ��̐錾�̏��Ԃ��قȂ�ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wreorder")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wreorder")
# # �I�[�o�[���[�h�ɂ����unsigned ~��enum���Asigned ~�Ɍ^�ϊ������ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wsign-promo")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wsign-promo")
# # -fstack-protector���w�肳��Ă���ꍇ�ɁA�X�^�b�N�ی삪�Ȃ���Ȃ������֐������݂���ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wstack-protector")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wstack-protector")
# # switch����default���x���̕��������Ȃ��ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wswitch-default")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wswitch-default")
# # switch���̑ΏۂɂȂ�l��enum�ŁA���x����enum�̂��ׂĂ̒l�ɂ͑Ή����Ă��Ȃ��ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wswitch-enum")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wswitch-enum")
# # ���[�v�J�E���^���I�[�o�[�t���[����\���������āA���[�v���œK���ł��Ȃ��ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wunsafe-loop-optimizations")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wunsafe-loop-optimizations")
# # �ڔ����̂��Ă��Ȃ����������_�����e�����ɑ΂��Čx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wunsuffixed-float-constants")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wunsuffixed-float-constants")
#
# # �ϐ����A���̕ϐ����g�̌^�ɃL���X�g���Ă���(���Ӗ��ł���)�ꍇ�Ɍx�����o���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wuseless-cast")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wuseless-cast")

# �����񃊃e����(const char*)��char*�֌^�ϊ�����ꍇ�Ɍx�����o���B
set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wwrite-strings")
set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wwrite-strings")

# # �������e����'0'���k���|�C���^�������萔�Ƃ��Ďg���Ă���ꍇ�Ɍx�����o�͂���B
# set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wzero-as-null-pointer-constant")
# set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wzero-as-null-pointer-constant")

# ----------------------------------------------------------------------

set(CROSS_PREFIX arm-linux-gnueabi-)
set(CROSS_TOOLCHAIN_PATH /usr/arm-linux-gnueabi)
set(CROSS_FLAGS_C "${CROSS_FLAGS_C} -Wall -Wno-unused-function -DSHARAKU_PROF_ENABLE -DSHARAKU_PROF_CLEAR_ENABLE")
set(CROSS_FLAGS_CXX "${CROSS_FLAGS_CXX} -Wall -Wno-unused-function -Wno-invalid-offsetof -DSHARAKU_PROF_ENABLE -DSHARAKU_PROF_CLEAR_ENABLE")

# �R���p�C���ݒ�
set(CMAKE_C_COMPILER ${CROSS_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${CROSS_PREFIX}g++)
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CROSS_TOOLCHAIN_PATH}/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CROSS_TOOLCHAIN_PATH}/lib)
set(CMAKE_INSTALL_PREFIX ${CROSS_TOOLCHAIN_PATH}/)
