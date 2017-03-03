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

