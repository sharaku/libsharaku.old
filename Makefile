# Copyright 2004-2016 abe takafumi All Rights Reserved.

lib-platform	= linux

all: arm x86

arm:
	$(MAKE) -C system/ clean lib-target=libsharaku-sys.arm.a
	$(MAKE) -C devices/ clean lib-target=libsharaku-dev.arm.a
	$(MAKE) -C modules/ clean lib-target=libsharaku-mod.arm.a
	$(MAKE) -C system/ lib-target=libsharaku-sys.arm.a lib-platform=$(lib-platform) CC=arm-linux-gnueabi-gcc CXX=arm-linux-gnueabi-g++ CFLAGS=-ffast-math
	$(MAKE) -C devices/ lib-target=libsharaku-dev.arm.a lib-platform=$(lib-platform) CC=arm-linux-gnueabi-gcc CXX=arm-linux-gnueabi-g++ CFLAGS=-ffast-math
	$(MAKE) -C modules/ lib-target=libsharaku-mod.arm.a lib-platform=$(lib-platform) CC=arm-linux-gnueabi-gcc CXX=arm-linux-gnueabi-g++ CFLAGS=-ffast-math

x86:
	$(MAKE) -C system/ clean lib-target=libsharaku-sys.x86.a 
	$(MAKE) -C devices/ clean lib-target=libsharaku-dev.x86.a
	$(MAKE) -C modules/ clean lib-target=libsharaku-mod.x86.a
	$(MAKE) -C system/ lib-target=libsharaku-sys.x86.a lib-platform=$(lib-platform) CC=gcc CXX=gcc
	$(MAKE) -C devices/ lib-target=libsharaku-dev.x86.a lib-platform=$(lib-platform) CC=gcc CXX=gcc
	$(MAKE) -C modules/ lib-target=libsharaku-mod.x86.a lib-platform=$(lib-platform) CC=gcc CXX=gcc

x86-testing:
	$(MAKE) -C system/ clean lib-target=libsharaku-sys.x86.testing.a 
	$(MAKE) -C devices/ clean lib-target=libsharaku-dev.x86.testing.a
	$(MAKE) -C modules/ clean lib-target=libsharaku-mod.x86.testing.a
	$(MAKE) -C system/ lib-target=libsharaku-sys.x86.testing.a lib-platform=$(lib-platform) CFLAGS=--coverage test-build=true CC=gcc CXX=gcc
	$(MAKE) -C devices/ lib-target=libsharaku-dev.x86.testing.a lib-platform=$(lib-platform) CFLAGS=--coverage test-build=true CC=gcc CXX=gcc
	$(MAKE) -C modules/ lib-target=libsharaku-mod.x86.testing.a lib-platform=$(lib-platform) CFLAGS=--coverage test-build=true CC=gcc CXX=gcc

clean:
	$(MAKE) -C system/ clean lib-target=libsharaku-sys.x86.a 
	$(MAKE) -C devices/ clean lib-target=libsharaku-dev.x86.a
	$(MAKE) -C modules/ clean lib-target=libsharaku-mod.x86.a
	$(MAKE) -C system/ clean lib-target=libsharaku-sys.arm.a 
	$(MAKE) -C devices/ clean lib-target=libsharaku-dev.arm.a
	$(MAKE) -C modules/ clean lib-target=libsharaku-mod.arm.a
	$(MAKE) -C system/ clean lib-target=libsharaku-sys.x86.testing.a 
	$(MAKE) -C devices/ clean lib-target=libsharaku-dev.x86.testing.a
	$(MAKE) -C modules/ clean lib-target=libsharaku-mod.x86.testing.a

