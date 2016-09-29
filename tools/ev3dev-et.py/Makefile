# Copyright 2016 abe takafumi All Rights Reserved.

# lib-platform

obj-hackev-target	:= HackEV.so
obj-ev3way-target	:= EV3Way.so

SHARAKU		:= ../..
INCLUDE_PATH	:= $(SHARAKU)/include
CC		:= arm-linux-gnueabi-gcc
CXX		:= arm-linux-gnueabi-g++

# libsharaku‚Ìƒ‰ƒCƒuƒ‰ƒŠ
lib-flags	+= -L$(SHARAKU)/devices/ -lsharaku-dev.arm
lib-flags	+= -L$(SHARAKU)/modules/ -lsharaku-mod.arm
lib-flags	+= -L$(SHARAKU)/system/ -lsharaku-sys.arm
lib-flags	+= -lpthread
lib-flags	+= -lboost_python
lib-flags	+= -lpython2.7
lib-flags	+= -DPIC -shared -fPIC

obj-includes   	:= -I$(INCLUDE_PATH)
obj-includes   	+= -I/usr/arm-linux-gnueabi/include
obj-includes   	+= -I/usr/arm-linux-gnueabi/include/python2.7

obj-defines	+= -Wall -g -O -Wno-unused-function
obj-defines	+= -Wno-unused-variable

src-hackev	+= EV3Way.cpp
src-ev3way	+= HackEV.cpp

obj-hackev	:= $(src-hackev:.cpp=.o)
obj-ev3way	:= $(src-ev3way:.cpp=.o)

all: $(obj-hackev-target) $(obj-ev3way-target)

.c.o:
	$(CC) $(CFLAGS) $(obj-includes) $(obj-defines) -c $<
.cpp.o:
	$(CXX) $(CFLAGS) $(obj-includes) $(obj-defines) -c $<

$(obj-hackev-target): $(notdir $(obj-hackev))
	$(CXX) $(notdir $(obj-hackev)) -o $(obj-hackev-target) $(lib-flags)
	@echo "$(lib-target) make success"

$(obj-ev3way-target): $(notdir $(obj-ev3way))
	$(CXX) $(notdir $(obj)) -o $(obj-ev3way-target) $(lib-flags)
	@echo "$(lib-target) make success"

clean:
	$(RM) $(obj-hackev-target) $(notdir $(obj-hackev))
	$(RM) $(obj-ev3way-target) $(notdir $(obj-ev3way))
