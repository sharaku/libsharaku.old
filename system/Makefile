# Copyright 2004-2016 abe takafumi All Rights Reserved.

# lib-target
# lib-platform

include configs/sharaku.config

SHARAKU		:= ..
ARFLAG		:= r

lib-includes   	+= $(SHARAKU)/include

lib-defines	+= $(if $(filter y,$(CONFIG_PROFILE)), SHARAKU_PROF_ENABLE)
lib-defines	+= $(if $(filter y,$(CONFIG_PROFILE_CLEAR)), SHARAKU_PROF_CLEAR_ENABLE)
lib-flags	+= $(if $(CONFIG_O), -O$(CONFIG_O))
lib-flags	+= -g
lib-flags	+= -Wall
#lib-flags	+= -Wextra
lib-flags	+= -Wno-unused-function

cc-opt		+= $(CFLAGS)
cc-opt		+= $(addprefix -I,$(lib-includes))
cc-opt		+= $(addprefix -D,$(lib-defines))
cc-opt		+= $(lib-flags)

lib-sys		+= sched/task.c
lib-sys		+= sched/sched.c
#lib-sys	+= sched/sched-$(lib-platform)-in.c
lib-sys		+= debug/log.c
lib-sys		+= debug/prof.c
lib-sys		+= activity.c
lib-sys		+= module.c

all: $(lib-target)

.c.o:
	$(CC) $(cc-opt) -c $<

$(lib-target):  $(lib-sys:.c=.o)
	$(AR) $(ARFLAG) $(lib-target) $(notdir $(lib-sys:.c=.o))
	@echo "$(lib-target) make success"

clean:
	$(RM) $(lib-target) $(notdir $(lib-sys:.c=.o))
