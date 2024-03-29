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
	libEV3Way.cpp
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
	libHackEV.cpp
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
