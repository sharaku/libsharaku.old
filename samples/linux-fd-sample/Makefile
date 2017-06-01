# Copyright 2017 abe takafumi All Rights Reserved.

# lib-platform

obj-target	:= linux_fd_poll_sample

SHARAKU_DEPROY_PATH	:= ../../tools/integration/deproy/libsharaku
INCLUDE_PATH	:= $(SHARAKU_DEPROY_PATH)/usr/local/include/libsharaku/x86-linux/
CFLAGS    	:= -Wall -g -Wunused-function

lib-flags	+= -L$(SHARAKU_DEPROY_PATH)/usr/local/lib/libsharaku/x86-linux/
lib-flags	+= -lsharaku-sys.linux.x86
lib-flags	+= -lpthread

obj-includes   	:= -I$(INCLUDE_PATH)

obj-defines	+= $(if $(CONFIG_O), -O$(CONFIG_O))
obj-defines	+= -Wno-unused-variable
obj-defines	+= -Wno-invalid-offsetof
obj-defines	+= -Wno-unused-but-set-variable

src		+= sample_fd.c
obj		:= $(src:.c=.o)

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
