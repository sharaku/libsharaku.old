# ----------------------------------------------------------------------------
#
#  Copyright Abe Takafumi All Rights Reserved, 2016
#  Author Abe Takafumi
#
#  Devices CMake file for libsharaku
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5)

# ---------------------------------------------------------------
include_directories(${SHARAKU_INCLUDE_PATH})

set(DEVICE_PROC
	""
	)
set(DEVICE_COLOR
	"color/hook-color.cpp"
	)
set(DEVICE_GYRO
	"gyro/hook-gyro.cpp"
	)
set(DEVICE_MOTOR
	"motor/hook-motor.cpp"
	)
set(DEVICE_POWER
	"power/hook-powersupply.cpp"
	)
set(DEVICE_TOUCH
	"touch/hook-touch.cpp"
	)
set(DEVICE_USONIC
	"usonic/hook-ultrasonic.cpp"
	)
set(DEVICE_SD
	"sd/sd-balancer.cpp"
	"sd/sd-linetrace.cpp"
	"sd/sd-linetrace-pid.cpp"
	"sd/sd-motors.cpp"
	"sd/sd-position-move.cpp"
	"sd/sd-wheel-odometry.cpp"
	)

if (${TARGET_OS} STREQUAL "linux")
	# Linux用ビルド
	set(DEVICE_PROC ${DEVICE_PROC}
		"proc/linux-proc-device.cpp"
		)
endif()


if (${TARGET_PLATFORM} STREQUAL "ev3dev")
	# ev3dev用ビルド
	set(DEVICE_PROC ${DEVICE_PROC}
		"proc/linux-proc-device.cpp"
		"proc/ev3dev-proc-device.cpp"
		)
	set(DEVICE_COLOR ${DEVICE_COLOR}
		"color/ev3dev-color.cpp"
		)
	set(DEVICE_GYRO ${DEVICE_GYRO}
		"gyro/ev3dev-gyro.cpp"
		)
	set(DEVICE_MOTOR ${DEVICE_MOTOR}
		"motor/ev3dev-tacho-motor.cpp"
		)
	set(DEVICE_POWER ${DEVICE_POWER}
		"power/ev3dev-power-supply.cpp"
		)
	set(DEVICE_TOUCH ${DEVICE_TOUCH}
		"touch/ev3dev-touch.cpp"
		)
	set(DEVICE_USONIC ${DEVICE_USONIC}
		"usonic/ev3dev-ultrasonic.cpp"
		)
	set(DEVICE_SD ${DEVICE_SD}
		"sd/ev3way/balancer.c"
		"sd/ev3way/balancer_param.c"
		"sd/ev3way/ev3way-balancer.cpp"
		)
elseif (${TARGET_PLATFORM} STREQUAL "ev3rt")
	# EV3RT用ビルド
	set(DEVICE_SD ${DEVICE_SD}
		"sd/ev3way/balancer.c"
		"sd/ev3way/balancer_param.c"
		"sd/ev3way/ev3way-balancer.cpp"
		)
endif()

add_library(sharaku-dev.${TARGET_SUFFIX} STATIC
	${DEVICE_PROC}
	${DEVICE_COLOR}
	${DEVICE_GYRO}
	${DEVICE_MOTOR}
	${DEVICE_POWER}
	${DEVICE_TOUCH}
	${DEVICE_USONIC}
	${DEVICE_SD}
	)
