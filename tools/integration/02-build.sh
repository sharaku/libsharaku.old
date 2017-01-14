#!/bin/sh

rm -rf build
mkdir build
cd build

# x86-linuxビルドを行う
do_x86_linux_build()
{
	rm -f CMakeCache.txt cmake_install.cmake rm Makefile
	rm -rf CMakeFiles
	cmake ../../../ -DCMAKE_TOOLCHAIN_FILE=tools/cmake/linux-x86.cmake
	make > ../result/make.result.linux-x86 2>&1
}


# arm-linuxビルドを行う
do_arm_linux_build()
{
	rm -f CMakeCache.txt cmake_install.cmake rm Makefile
	rm -rf CMakeFiles
	cmake ../../../ -DCMAKE_TOOLCHAIN_FILE=tools/cmake/ev3dev-arm.cmake
	make > ../result/make.result.ev3dev-arm 2>&1
}

# arm-itronビルドを行う
do_arm_itron_build()
{
	rm -f CMakeCache.txt cmake_install.cmake rm Makefile
	rm -rf CMakeFiles
	cmake ../../../ -DCMAKE_TOOLCHAIN_FILE=tools/cmake/ev3rt-arm.cmake
	make > ../result/make.result.ev3rt-arm 2>&1
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
