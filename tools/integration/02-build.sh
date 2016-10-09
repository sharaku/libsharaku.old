#!/bin/sh

mkdir -p result
make -C ../../ clear

# libsharaku builds
make -C ../../ x86 2> result/result-build.x86.txt
make -C ../../ arm 2> result/result-build.arm.txt

# testing builds
make -C ../../ x86-testing 2> result/result-build.x86-testing.txt

# ev3dev tools builds
make -C ../ev3dev-et.py clean
make -C ../ev3dev-et.py CC=arm-linux-gnueabi-gcc CXX=arm-linux-gnueabi-g++

