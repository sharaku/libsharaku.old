#!/bin/bash

wget https://sourceforge.net/projects/boost/files/boost/1.61.0/boost_1_61_0.tar.bz2
tar --bzip2 -xf boost_1_61_0.tar.bz2
cd boost_1_61_0

echo "using gcc : arm : arm-linux-gnueabi-g++ ;" > $HOME/user-config.jam
./bootstrap.sh
./b2 toolset=gcc-arm -j2 architecture=arm address-model=32 --prefix=/usr/arm-linux-gnueabi/ include=/usr/arm-linux-gnueabi/include/python2.7/ --clean
./b2 toolset=gcc-arm -j2 architecture=arm address-model=32 --prefix=/usr/arm-linux-gnueabi/ include=/usr/arm-linux-gnueabi/include/python2.7/ --with-python install

cd ../
rm -rf boost_1_61_0
rm -f boost_1_61_0.tar.bz2

exit 0
