#!/bin/sh
# ----------------------------------------------------------------------------
#
#  MIT License
#  
#  Copyright (c) 2017 Abe Takafumi
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
# ----------------------------------------------------------------------------

rm -rf build
mkdir build
cd build

# x86-linuxビルドを行う
do_x86_linux_build()
{
	rm -f CMakeCache.txt cmake_install.cmake rm Makefile
	rm -rf CMakeFiles
	cmake ../../../ -DCMAKE_TOOLCHAIN_FILE=tools/cmake/linux-x86.cmake
	make 2>&1 | tee ../result/make.result.linux-x86
}


# arm-linuxビルドを行う
do_arm_linux_build()
{
	rm -f CMakeCache.txt cmake_install.cmake rm Makefile
	rm -rf CMakeFiles
	cmake ../../../ -DCMAKE_TOOLCHAIN_FILE=tools/cmake/ev3dev-arm.cmake
	make 2>&1 | tee ../result/make.result.ev3dev-arm
}

# arm-itronビルドを行う
do_arm_itron_build()
{
	rm -f CMakeCache.txt cmake_install.cmake rm Makefile
	rm -rf CMakeFiles
	cmake ../../../ -DCMAKE_TOOLCHAIN_FILE=tools/cmake/ev3rt-arm.cmake
	make 2>&1 | tee ../result/make.result.ev3rt-arm
}

case $1 in
	x86-linux )
		do_x86_linux_build
		;;
	arm-linux )
		do_arm_linux_build
		;;
	arm-itron )
		do_arm_itron_build
		;;
	* )
		do_x86_linux_build
		do_arm_linux_build
		do_arm_itron_build
		;;
esac
