#!/bin/sh

rm -rf build
mkdir build
cd build

# arm-linuxビルドを行う
rm -f CMakeCache.txt cmake_install.cmake rm Makefile
rm -rf CMakeFiles
cmake ../../../ -DCMAKE_TOOLCHAIN_FILE=tools/cmake/ev3dev-arm.cmake
make

# arm-itronビルドを行う
rm -f CMakeCache.txt cmake_install.cmake rm Makefile
rm -rf CMakeFiles
cmake ../../../ -DCMAKE_TOOLCHAIN_FILE=tools/cmake/ev3rt-arm.cmake
make
