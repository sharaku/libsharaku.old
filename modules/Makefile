# Copyright 2004-2016 abe takafumi All Rights Reserved.

# lib-target
# lib-platform

include configs/sharaku.config

SHARAKU		:= ..
INCLUDE_PATH	:= $(SHARAKU)/include
ARFLAG		:= r

lib-includes   	:= -I$(INCLUDE_PATH)

lib-defines	+= $(if $(CONFIG_PROFILE), -DSHARAKU_PROF_ENABLE)
lib-defines	+= $(if $(CONFIG_PROFILE_CLEAR), -DSHARAKU_PROF_CLEAR_ENABLE)
lib-defines	+= $(if $(CONFIG_O), -O$(CONFIG_O))
lib-defines	+= -g
lib-defines	+= -Wall
lib-defines	+= -Wno-unused-function
lib-defines	+= -Wno-unused-variable
lib-defines	+= -Wno-invalid-offsetof
lib-defines	+= -Wno-unused-but-set-variable

lib-mod		+= mod-update/mod-update.cpp

lib-obj		:= $(lib-mod:.c=.o)
lib-obj		:= $(lib-obj:.cpp=.o)

all: $(lib-target)

.c.o:
	$(CC) $(CFLAGS) $(lib-includes) $(lib-defines) -c $<
.cpp.o:
	$(CXX) $(CFLAGS) $(lib-includes) $(lib-defines) -c $<

$(lib-target): $(lib-obj)
	$(AR) $(ARFLAG) $(lib-target) $(notdir $(lib-obj))
	@echo "$(lib-target) make success"

clean:
	$(RM) $(lib-target) $(notdir $(lib-obj))
