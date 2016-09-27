#!/bin/sh

CC=arm-linux-gnueabi-gcc
CXX=arm-linux-gnueabi-g++

# clean up
make -C ../../ clean
make -C ../ev3dev-et.py clean

# builds
make -C ../../ CC=${CC} CXX=${CXX} 
make -C ../ev3dev-et.py CC=${CC} CXX=${CXX}

