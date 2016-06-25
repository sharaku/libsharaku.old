# Copyright 2004-2016 abe takafumi All Rights Reserved.

# lib-platform

obj-target	:= ev3dev_sample

SHARAKU		:= ../..
INCLUDE_PATH	:= $(SHARAKU)/include
CFLAGS    	:= -Wall -g -Wunused-function

lib-flags	+= -L../../devices/ -lsharaku-dev
lib-flags	+= -L../../modules/ -lsharaku-mod
lib-flags	+= -L../../system/ -lsharaku-sys
lib-flags	+= -lpthread

obj-includes   	:= -I$(INCLUDE_PATH)

obj-defines	+= $(if $(CONFIG_O), -O$(CONFIG_O))
obj-defines	+= -Wno-unused-variable
obj-defines	+= -Wno-invalid-offsetof
obj-defines	+= -Wno-unused-but-set-variable

src		+= main.cpp

obj		:= $(src:.c=.o)
obj		:= $(obj:.cpp=.o)

all: $(obj-target)

.c.o:
	$(CC) $(CFLAGS) $(obj-includes) $(obj-defines) -c $<
.cpp.o:
	$(CXX) $(CFLAGS) $(obj-includes) $(obj-defines) -c $<

$(obj-target): $(notdir $(obj))
	$(CXX) $(notdir $(obj)) -o $(obj-target) $(lib-flags)
	@echo "$(lib-target) make success"

clean:
	$(RM) $(obj-target) $(notdir $(obj))
