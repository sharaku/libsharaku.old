#!/bin/bash

wget http://www.python.org/ftp/python/2.7.3/Python-2.7.3.tar.bz2
wget https://raw.githubusercontent.com/sjkingo/python-arm-xcompile/master/files/Python-2.7.3-xcompile.patch

tar xf Python-2.7.3.tar.bz2 && cd Python-2.7.3
./configure
make python Parser/pgen
mv python buildpython
mv Parser/pgen buildpgen
make distclean
patch -p1 < ../Python-2.7.3-xcompile.patch

CC=arm-linux-gnueabi-gcc CXX=arm-linux-gnueabi-g++ AR=arm-linux-gnueabi-ar RANLIB=arm-linux-gnueabi-ranlib BASECFLAGS=-mcpu=arm926ej-s \
./configure --host=arm-linux-gnueabi --build=x86_64-linux-gnu --prefix=/usr/arm-linux-gnueabi

make HOSTPYTHON=./buildpython HOSTPGEN=./buildpgen BLDSHARED="arm-linux-gnueabi-gcc -shared" CROSS_COMPILE=arm-linux-gnueabi- CROSS_COMPILE_TARGET=yes HOSTARCH=arm-linux-gnueabi BUILDARCH=x86_64-linux-gnu
make install HOSTPYTHON=./buildpython CROSS_COMPILE=arm-none-linux-gnueabi- BLDSHARED="arm-linux-gnueabi-gcc -shared" CROSS_COMPILE_TARGET=yes

cd ../
rm -rf Python-2.7.3
rm -f Python-2.7.3.tar.bz2

exit 0
