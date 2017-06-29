/* --
 *
 * MIT License
 * 
 * Copyright (c) 2014-2016 Abe Takafumi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. *
 *
 */

#ifndef SHARAKU_DEVICES_EV3DEV_IN_H_
#define SHARAKU_DEVICES_EV3DEV_IN_H_


#define DEVICE_FILEPATH_MAXLEN	128
#define DEVICE_BUFFER_LEN	16
#include <stdint.h>
#include <sys/types.h>
#include <dirent.h>
#include <sharaku/sched.h>
#include <devices/proc/linux-proc-device.hpp>

NAMESPACE_SHARAKU_BEGIN

static const char EV3_INPUT_1[]  = "in1\n";  //!< Sensor port 1
static const char EV3_INPUT_2[]  = "in2\n";  //!< Sensor port 2
static const char EV3_INPUT_3[]  = "in3\n";  //!< Sensor port 3
static const char EV3_INPUT_4[]  = "in4\n";  //!< Sensor port 4
 
static const char EV3_OUTPUT_A[] = "outA\n"; //!< Motor port A
static const char EV3_OUTPUT_B[] = "outB\n"; //!< Motor port B
static const char EV3_OUTPUT_C[] = "outC\n"; //!< Motor port C
static const char EV3_OUTPUT_D[] = "outD\n"; //!< Motor port D

#if 1
#define EV3DEV_TACHOMOTOR_BASE "/sys/class/tacho-motor/"
#define EV3DEV_MSENSOR_BASE "/sys/class/lego-sensor/"
#define EV3DEV_POWER_BASE "/sys/class/power_supply/legoev3-battery/"
#else
#define EV3DEV_TACHOMOTOR_BASE "/tmp/libsharaku/tacho-motor/"
#define EV3DEV_MSENSOR_BASE "/tmp/libsharaku/lego-sensor/"
#define EV3DEV_POWER_BASE "/tmp/libsharaku/legoev3-battery/"
#endif

int32_t ev3dev_lego_get_device_path(char* outpath, int outpath_len,
			    const char* device_path, const char* port,
			    const char* file);

#define TACHOMOTOR_CONNECT(RET, P, F)					\
	ev3dev_lego_get_device_path(buffer, DEVICE_FILEPATH_MAXLEN, EV3DEV_TACHOMOTOR_BASE, P, #F);	\
	*RET = F.connect(buffer);

#define TACHOMOTOR_CONNECT2(RET, P, PATH, F)					\
	ev3dev_lego_get_device_path(buffer, DEVICE_FILEPATH_MAXLEN, EV3DEV_TACHOMOTOR_BASE, P, PATH);	\
	*RET = F.connect(buffer);

#define MSENSOR_CONNECT(RET, P, F) 					\
	ev3dev_lego_get_device_path(buffer, DEVICE_FILEPATH_MAXLEN, EV3DEV_MSENSOR_BASE, P, #F);	\
	*RET = F.connect(buffer);

#define POWER_CONNECT(RET, F)						\
	*RET = F.connect(EV3DEV_POWER_BASE#F);

#define DEVICE_CONNECT_BEGIN				\
	int32_t connect(const char* ___port___) {		\
		int32_t ___result___; 			\
		char buffer[DEVICE_FILEPATH_MAXLEN];	\
		do {

#define DEVICE_TACHOMOTOR_CONNECT(NAME)					\
			TACHOMOTOR_CONNECT(&___result___, ___port___, NAME);	\
			if (___result___) break;

#define DEVICE_TACHOMOTOR_CONNECT2(PATH, NAME)				\
			TACHOMOTOR_CONNECT2(&___result___, ___port___, PATH, NAME);	\
			if (___result___) break;

#define DEVICE_MSENSOR_CONNECT(NAME)					\
			MSENSOR_CONNECT(&___result___, ___port___, NAME);	\
			if (___result___) break;

#define DEVICE_CONNECT_END		\
		} while(0);		\
		start();		\
		return ___result___;	\
	}

#define DEVICE_POWER_CONNECT_BEGIN		\
	int32_t connect(void) {			\
		int32_t ___result___; 			\
		do {

#define DEVICE_POWER_CONNECT(NAME)					\
			POWER_CONNECT(&___result___, NAME);		\
			if (___result___) break;

#define DEVICE_POWER_CONNECT_END	\
		} while(0);		\
		start();		\
		return ___result___;	\
	}


class ev3dev_lego_tacho_motor
 : public linux_proc_device {
 public:
	enum {
		DEVICE_FILE_READ_ADDERSS		= 0x00000001,
		// DEVICE_FILE_READ_COMMAND		= 0x00000002,
		DEVICE_FILE_READ_COMMANDS		= 0x00000004,
		DEVICE_FILE_READ_COUNT_PER_ROT		= 0x00000008,
		DEVICE_FILE_READ_DRIVER_NAME		= 0x00000010,
		DEVICE_FILE_READ_DUTY_CYCLE		= 0x00000020,
		DEVICE_FILE_READ_DUTY_CYCLE_SP		= 0x00000040,
		DEVICE_FILE_READ_ENCODER_POLARTY	= 0x00000080,
		DEVICE_FILE_READ_POLARITY		= 0x00000100,
		DEVICE_FILE_READ_POSITION		= 0x00000200,
		DEVICE_FILE_READ_HOLD_PID_KD		= 0x00000400,
		DEVICE_FILE_READ_HOLD_PID_KI		= 0x00000800,
		DEVICE_FILE_READ_HOLD_PID_KP		= 0x00001000,
		DEVICE_FILE_READ_POSITION_SP		= 0x00002000,
		DEVICE_FILE_READ_SPEED			= 0x00004000,
		DEVICE_FILE_READ_SPEED_SP		= 0x00008000,
		DEVICE_FILE_READ_RAMP_UP_SP		= 0x00010000,
		DEVICE_FILE_READ_RAMP_DOWN_SP		= 0x00020000,
		DEVICE_FILE_READ_SPEED_REGULATION	= 0x00040000,
		DEVICE_FILE_READ_SPEED_PID_KD		= 0x00080000,
		DEVICE_FILE_READ_SPEED_PID_KI		= 0x00100000,
		DEVICE_FILE_READ_SPEED_PID_KP		= 0x00200000,
		DEVICE_FILE_READ_STATE			= 0x00400000,
		DEVICE_FILE_READ_STOP_COMMAND		= 0x00800000,
		DEVICE_FILE_READ_STOP_COMMANDS		= 0x01000000,
		DEVICE_FILE_READ_TIME_SP		= 0x02000000,

		// DEVICE_FILE_WRITE_ADDERSS		= 0x00000001,
		DEVICE_FILE_WRITE_COMMAND		= 0x00000002,
		// DEVICE_FILE_WRITE_COMMANDS		= 0x00000004,
		// DEVICE_FILE_WRITE_COUNT_PER_ROT	= 0x00000008,
		// DEVICE_FILE_WRITE_DRIVER_NAME	= 0x00000010,
		// DEVICE_FILE_WRITE_DUTY_CYCLE		= 0x00000020,
		DEVICE_FILE_WRITE_DUTY_CYCLE_SP		= 0x00000040,
		DEVICE_FILE_WRITE_ENCODER_POLARTY	= 0x00000080,
		DEVICE_FILE_WRITE_POLARITY		= 0x00000100,
		DEVICE_FILE_WRITE_POSITION		= 0x00000200,
		DEVICE_FILE_WRITE_HOLD_PID_KD		= 0x00000400,
		DEVICE_FILE_WRITE_HOLD_PID_KI		= 0x00000800,
		DEVICE_FILE_WRITE_HOLD_PID_KP		= 0x00001000,
		DEVICE_FILE_WRITE_POSITION_SP		= 0x00002000,
		// DEVICE_FILE_WRITE_SPEED		= 0x00004000,
		DEVICE_FILE_WRITE_SPEED_SP		= 0x00008000,
		DEVICE_FILE_WRITE_RAMP_UP_SP		= 0x00010000,
		DEVICE_FILE_WRITE_RAMP_DOWN_SP		= 0x00020000,
		DEVICE_FILE_WRITE_SPEED_REGULATION	= 0x00040000,
		DEVICE_FILE_WRITE_SPEED_PID_KD		= 0x00080000,
		DEVICE_FILE_WRITE_SPEED_PID_KI		= 0x00100000,
		DEVICE_FILE_WRITE_SPEED_PID_KP		= 0x00200000,
		// DEVICE_FILE_WRITE_STATE		= 0x00400000,
		DEVICE_FILE_WRITE_STOP_COMMAND		= 0x00800000,
		// DEVICE_FILE_WRITE_STOP_COMMANDS	= 0x01000000,
		DEVICE_FILE_WRITE_TIME_SP		= 0x02000000,
	};

	ev3dev_lego_tacho_motor(uint32_t interval_ms)
	 : linux_proc_device(interval_ms) {}

	DEVICE_CONNECT_BEGIN
		DEVICE_TACHOMOTOR_CONNECT(address)
		DEVICE_TACHOMOTOR_CONNECT(command)
		DEVICE_TACHOMOTOR_CONNECT(commands)
		DEVICE_TACHOMOTOR_CONNECT(count_per_rot)
		DEVICE_TACHOMOTOR_CONNECT(driver_name)
		DEVICE_TACHOMOTOR_CONNECT(duty_cycle)
		DEVICE_TACHOMOTOR_CONNECT(duty_cycle_sp)
		DEVICE_TACHOMOTOR_CONNECT(encoder_polarity)
		DEVICE_TACHOMOTOR_CONNECT(polarity)
		DEVICE_TACHOMOTOR_CONNECT(position)
		DEVICE_TACHOMOTOR_CONNECT2("hold_pid/Kd", hold_pid_Kd)
		DEVICE_TACHOMOTOR_CONNECT2("hold_pid/Ki", hold_pid_Ki)
		DEVICE_TACHOMOTOR_CONNECT2("hold_pid/Kp", hold_pid_Kp)
		DEVICE_TACHOMOTOR_CONNECT(position_sp)
		DEVICE_TACHOMOTOR_CONNECT(speed)
		DEVICE_TACHOMOTOR_CONNECT(speed_sp)
		DEVICE_TACHOMOTOR_CONNECT(ramp_up_sp)
		DEVICE_TACHOMOTOR_CONNECT(ramp_down_sp)
		DEVICE_TACHOMOTOR_CONNECT(speed_regulation)
		DEVICE_TACHOMOTOR_CONNECT2("speed_pid/Kd", speed_pid_Kd)
		DEVICE_TACHOMOTOR_CONNECT2("speed_pid/Ki", speed_pid_Ki)
		DEVICE_TACHOMOTOR_CONNECT2("speed_pid/Kp", speed_pid_Kp)
		DEVICE_TACHOMOTOR_CONNECT(state)
		DEVICE_TACHOMOTOR_CONNECT(stop_command)
		DEVICE_TACHOMOTOR_CONNECT(stop_commands)
		DEVICE_TACHOMOTOR_CONNECT(time_sp)
	DEVICE_CONNECT_END

	DEVICE_IO_SUBMIT_BEGIN
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_COUNT_PER_ROT, count_per_rot)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_DRIVER_NAME, driver_name)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_DUTY_CYCLE, duty_cycle)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_DUTY_CYCLE_SP, duty_cycle_sp)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_ENCODER_POLARTY, encoder_polarity)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_POLARITY, polarity)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_POSITION, position)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_HOLD_PID_KD, hold_pid_Kd)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_HOLD_PID_KI, hold_pid_Ki)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_HOLD_PID_KP, hold_pid_Kp)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_POSITION_SP, position_sp)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_SPEED, speed)
		DEVICE_READ_SUBMIT(DEVICE_FILE_WRITE_SPEED_SP, speed_sp)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_RAMP_UP_SP, ramp_up_sp)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_RAMP_DOWN_SP, ramp_down_sp)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_SPEED_REGULATION, speed_regulation)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_SPEED_PID_KD, speed_pid_Kd)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_SPEED_PID_KI, speed_pid_Ki)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_SPEED_PID_KP, speed_pid_Kp)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_STATE, state)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_STOP_COMMAND, stop_command)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_STOP_COMMANDS, stop_commands)
		DEVICE_READ_SUBMIT(DEVICE_FILE_READ_TIME_SP, time_sp)

		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_DUTY_CYCLE_SP, duty_cycle_sp)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_ENCODER_POLARTY, encoder_polarity)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_POLARITY, polarity)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_POSITION, position)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_HOLD_PID_KD, hold_pid_Kd)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_HOLD_PID_KI, hold_pid_Ki)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_HOLD_PID_KP, hold_pid_Kp)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_POSITION_SP, position_sp)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_SPEED_SP, speed_sp)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_RAMP_UP_SP, ramp_up_sp)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_RAMP_DOWN_SP, ramp_down_sp)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_SPEED_REGULATION, speed_regulation)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_SPEED_PID_KD, speed_pid_Kd)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_SPEED_PID_KI, speed_pid_Ki)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_SPEED_PID_KP, speed_pid_Kp)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_STOP_COMMAND, stop_command)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_TIME_SP, time_sp)

		DEVICE_WRITE_SUBMIT(DEVICE_FILE_WRITE_COMMAND, command)
	DEVICE_IO_SUBMIT_END

	void disconnect() {
		stop();
	}

 protected:
	device_accessor_r	address;
	device_accessor_w	command;
	device_accessor_r	commands;
	device_accessor_r	count_per_rot;
	device_accessor_r	driver_name;
	device_accessor_r	duty_cycle;
	device_accessor_rw	duty_cycle_sp;
	device_accessor_rw	encoder_polarity;
	device_accessor_rw	polarity;
	device_accessor_rw	position;
	device_accessor_rw	hold_pid_Kd;
	device_accessor_rw	hold_pid_Ki;
	device_accessor_rw	hold_pid_Kp;
	device_accessor_rw	position_sp;
	device_accessor_r	speed;
	device_accessor_rw	speed_sp;
	device_accessor_rw	ramp_up_sp;
	device_accessor_rw	ramp_down_sp;
	device_accessor_rw	speed_regulation;
	device_accessor_rw	speed_pid_Kd;
	device_accessor_rw	speed_pid_Ki;
	device_accessor_rw	speed_pid_Kp;
	device_accessor_r	state;
	device_accessor_rw	stop_command;
	device_accessor_r	stop_commands;
	device_accessor_rw	time_sp;

 private:
	ev3dev_lego_tacho_motor()
	 : linux_proc_device(1) {}
};

// gyro, color, touch, sonic, ...
class ev3dev_lego_sensor
 : public linux_proc_device {
 public:
	enum {
		DEVICE_FILE_ADDRESS		= 0x00000001,
		DEVICE_FILE_BIN_DATA		= 0x00000002,
		DEVICE_FILE_BIN_DATA_FORMAT	= 0x00000004,
		DEVICE_FILE_COMMAND		= 0x00000008,
		DEVICE_FILE_DIRECT		= 0x00000010,
		DEVICE_FILE_DECIMALS		= 0x00000020,
		DEVICE_FILE_DEVICE_NAME		= 0x00000040,
		DEVICE_FILE_FW_VERSION		= 0x00000080,
		DEVICE_FILE_MODE		= 0x00000100,
		DEVICE_FILE_MODES		= 0x00000200,
		DEVICE_FILE_NUM_VALUES		= 0x00000400,
		DEVICE_FILE_POLL_MS		= 0x00000800,
		DEVICE_FILE_UINTS		= 0x00001000,
		DEVICE_FILE_VALUE0		= 0x00002000,
		DEVICE_FILE_VALUE1		= 0x00004000,
		DEVICE_FILE_VALUE2		= 0x00008000,
		DEVICE_FILE_VALUE3		= 0x00010000,
		DEVICE_FILE_VALUE4		= 0x00020000,
		DEVICE_FILE_VALUE5		= 0x00040000,
		DEVICE_FILE_VALUE6		= 0x00080000,
		DEVICE_FILE_VALUE7		= 0x00100000,
		DEVICE_FILE_TEXT_VALUE		= 0x00200000,
	};

	ev3dev_lego_sensor(uint32_t interval_ms)
	 : linux_proc_device(interval_ms) {}

	DEVICE_CONNECT_BEGIN
		DEVICE_MSENSOR_CONNECT(address)
		DEVICE_MSENSOR_CONNECT(driver_name)
		DEVICE_MSENSOR_CONNECT(poll_ms)
		DEVICE_MSENSOR_CONNECT(mode)
		DEVICE_MSENSOR_CONNECT(modes)
		DEVICE_MSENSOR_CONNECT(value0)
		DEVICE_MSENSOR_CONNECT(value1)
		DEVICE_MSENSOR_CONNECT(value2)
		DEVICE_MSENSOR_CONNECT(value3)
		DEVICE_MSENSOR_CONNECT(value4)
		DEVICE_MSENSOR_CONNECT(value5)
		DEVICE_MSENSOR_CONNECT(value6)
		DEVICE_MSENSOR_CONNECT(value7)
	DEVICE_CONNECT_END

	DEVICE_IO_SUBMIT_BEGIN
		DEVICE_READ_SUBMIT(DEVICE_FILE_ADDRESS, address)
		DEVICE_READ_SUBMIT(DEVICE_FILE_DEVICE_NAME, driver_name)
		DEVICE_READ_SUBMIT(DEVICE_FILE_POLL_MS, poll_ms)
		DEVICE_WRITE_SUBMIT(DEVICE_FILE_MODE, mode)
		DEVICE_READ_SUBMIT(DEVICE_FILE_VALUE0, value0)
		DEVICE_READ_SUBMIT(DEVICE_FILE_VALUE1, value1)
		DEVICE_READ_SUBMIT(DEVICE_FILE_VALUE2, value2)
		DEVICE_READ_SUBMIT(DEVICE_FILE_VALUE3, value3)
		DEVICE_READ_SUBMIT(DEVICE_FILE_VALUE4, value4)
		DEVICE_READ_SUBMIT(DEVICE_FILE_VALUE5, value5)
		DEVICE_READ_SUBMIT(DEVICE_FILE_VALUE6, value6)
		DEVICE_READ_SUBMIT(DEVICE_FILE_VALUE7, value7)
	DEVICE_IO_SUBMIT_END

	void disconnect() {
		stop();
	}

 protected:
	device_accessor_r	address;
	device_accessor_r	bin_data;
	device_accessor_r	bin_data_format;
	device_accessor_w	command;
	device_accessor_r	commands;
	device_accessor_rw	direct;
	device_accessor_r	decimals;
	device_accessor_r	driver_name;
	device_accessor_r	fw_version;
	device_accessor_rw	mode;
	device_accessor_r	modes;
	device_accessor_r	num_values;
	device_accessor_rw	poll_ms;
	device_accessor_r	units;
	device_accessor_r	value0;
	device_accessor_r	value1;
	device_accessor_r	value2;
	device_accessor_r	value3;
	device_accessor_r	value4;
	device_accessor_r	value5;
	device_accessor_r	value6;
	device_accessor_r	value7;
	device_accessor_r	text_value;

 private:
	ev3dev_lego_sensor()
	 : linux_proc_device(1) {}
};

class ev3dev_lego_powersupply
 : public linux_proc_device {
 public:
	enum {
		DEVICE_FILE_CHARGE_FULL		= 0x00000001,
		DEVICE_FILE_CHARGE_NOW		= 0x00000002,
		DEVICE_FILE_CURRENT_NOW		= 0x00000004,
		DEVICE_FILE_VOLTAGE_NOW		= 0x00000008,
		DEVICE_FILE_VOLTAGE_MAX		= 0x00000010,
		DEVICE_FILE_VOLTAGE_MIN		= 0x00000020,
		DEVICE_FILE_STATUS		= 0x00000040,
		DEVICE_FILE_TYPE		= 0x00000080,
	};

	ev3dev_lego_powersupply(uint32_t interval_ms)
	 : linux_proc_device(interval_ms) {}

	DEVICE_POWER_CONNECT_BEGIN
		DEVICE_POWER_CONNECT(charge_full_design)
		DEVICE_POWER_CONNECT(charge_now)
		DEVICE_POWER_CONNECT(current_now)
		DEVICE_POWER_CONNECT(voltage_now)
		DEVICE_POWER_CONNECT(voltage_max_design)
		DEVICE_POWER_CONNECT(voltage_min_design)
		DEVICE_POWER_CONNECT(status)
		DEVICE_POWER_CONNECT(type)
	DEVICE_POWER_CONNECT_END

	DEVICE_IO_SUBMIT_BEGIN
		DEVICE_READ_SUBMIT(DEVICE_FILE_CHARGE_FULL, charge_full_design)
		DEVICE_READ_SUBMIT(DEVICE_FILE_CHARGE_NOW, charge_now)
		DEVICE_READ_SUBMIT(DEVICE_FILE_CURRENT_NOW, current_now)
		DEVICE_READ_SUBMIT(DEVICE_FILE_VOLTAGE_NOW, voltage_now)
		DEVICE_READ_SUBMIT(DEVICE_FILE_VOLTAGE_MAX, voltage_max_design)
		DEVICE_READ_SUBMIT(DEVICE_FILE_VOLTAGE_MIN, voltage_min_design)
		DEVICE_READ_SUBMIT(DEVICE_FILE_STATUS, status)
		DEVICE_READ_SUBMIT(DEVICE_FILE_TYPE, type)
	DEVICE_IO_SUBMIT_END

	void disconnect() {
		stop();
	}

 protected:
	device_accessor_r	charge_full_design;
	device_accessor_r	charge_now;
	device_accessor_r	current_now;
	device_accessor_r	voltage_now;
	device_accessor_r	voltage_max_design;
	device_accessor_r	voltage_min_design;
	device_accessor_r	status;
	device_accessor_r	type;

 private:
	ev3dev_lego_powersupply()
	 : linux_proc_device(1) {}
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_UV_DEVICE_EV3DEV_LARGE_MOTOR_H_
