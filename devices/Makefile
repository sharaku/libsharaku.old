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

lib-dev		+= proc/linux-proc-device.cpp
lib-dev		+= proc/ev3dev-proc-device.cpp
lib-dev		+= color/ev3dev-color.cpp
lib-dev		+= touch/ev3dev-touch.cpp
lib-dev		+= gyro/ev3dev-gyro.cpp
lib-dev		+= usonic/ev3dev-ultrasonic.cpp
lib-dev		+= motor/ev3dev-tacho-motor.cpp
lib-dev		+= power/ev3dev-power-supply.cpp

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
	$(CC) $(CFLAGS) $(lib-includes) $(lib-defines) -c $<
.cpp.o:
	$(CXX) $(CFLAGS) $(lib-includes) $(lib-defines) -c $<

$(lib-target): $(lib-obj)
	$(AR) $(ARFLAG) $(lib-target) $(notdir $(lib-obj))
	@echo "$(lib-target) make success"

clean:
	$(RM) $(lib-target) $(notdir $(lib-obj))
