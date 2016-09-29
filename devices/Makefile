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

lib-dev		+= proc/linux-proc-device.cpp
lib-dev		+= proc/ev3dev-proc-device.cpp
lib-dev		+= color/ev3dev-color.cpp
lib-dev		+= color/hook-color.cpp
lib-dev		+= touch/ev3dev-touch.cpp
lib-dev		+= touch/hook-touch.cpp
lib-dev		+= gyro/ev3dev-gyro.cpp
lib-dev		+= gyro/hook-gyro.cpp
lib-dev		+= usonic/ev3dev-ultrasonic.cpp
lib-dev		+= usonic/hook-ultrasonic.cpp
lib-dev		+= power/ev3dev-power-supply.cpp
lib-dev		+= power/hook-powersupply.cpp
lib-dev		+= motor/ev3dev-tacho-motor.cpp
lib-dev		+= motor/hook-motor.cpp

lib-dev		+= sd/sd-motors.cpp
lib-dev		+= sd/sd-wheel-odometry.cpp
lib-dev		+= sd/sd-linetrace.cpp
lib-dev		+= sd/sd-linetrace-pid.cpp
lib-dev		+= sd/sd-position-move.cpp
lib-dev		+= sd/sd-balancer.cpp

lib-dev		+= sd/ev3way/balancer.c
lib-dev		+= sd/ev3way/balancer_param.c
lib-dev		+= sd/ev3way/ev3way-balancer.cpp

lib-obj		:= $(lib-dev:.c=.o)
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
