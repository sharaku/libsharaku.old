# Copyright 2004-2016 abe takafumi All Rights Reserved.

# lib-target
# lib-platform

include configs/sharaku.config

SHARAKU		:= ..
ARFLAG		:= r

lib-includes   	+= $(SHARAKU)/include

lib-defines	+= $(if $(CONFIG_PROFILE), SHARAKU_PROF_ENABLE)
lib-defines	+= $(if $(CONFIG_PROFILE_CLEAR), SHARAKU_PROF_CLEAR_ENABLE)
lib-flags	+= $(if $(CONFIG_O), -O$(CONFIG_O))
lib-flags	+= -g
lib-flags	+= -Wall
lib-flags	+= -Wextra
lib-flags	+= -Wno-unused-function
lib-flags	+= -Wno-unused-variable
lib-flags	+= -Wno-invalid-offsetof
lib-flags	+= -Wno-unused-but-set-variable

cc-opt		+= $(CFLAGS)
cc-opt		+= $(addprefix -I,$(lib-includes))
cc-opt		+= $(addprefix -D,$(lib-defines))
cc-opt		+= $(lib-flags)

lib-mod		+= mod-update/mod-update.cpp

lib-obj		:= $(lib-mod:.c=.o)
lib-obj		:= $(lib-obj:.cpp=.o)

all: $(lib-target)

.c.o:
	$(CC) $(cc-opt) -c $<
.cpp.o:
	$(CXX) $(cc-opt) -c $<

$(lib-target): $(lib-obj)
	$(AR) $(ARFLAG) $(lib-target) $(notdir $(lib-obj))
	@echo "$(lib-target) make success"

clean:
	$(RM) $(lib-target) $(notdir $(lib-obj))
