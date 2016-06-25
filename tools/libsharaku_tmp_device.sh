#!/bin/bash


create_tacho_motor()
{
	base_path=/tmp/libsharaku/tacho-motor/motor${1}/
	mkdir ${base_path}

	echo ${2} > ${base_path}address
	echo stop > ${base_path}command
	echo run-forever run-to-abs-pos run-to-rel-pos run-timed run-direct stop reset > ${base_path}commands
	echo 0 > ${base_path}count_per_rot
	echo  > ${base_path}driver_name
	echo 0 > ${base_path}duty_cycle
	echo 0 > ${base_path}duty_cycle_sp
	echo normal > ${base_path}encoder_polarity
	echo normal > ${base_path}polarity
	echo 0 > ${base_path}position
	echo 0 > ${base_path}hold_pid_Kd
	echo 0 > ${base_path}hold_pid_Ki
	echo 0 > ${base_path}hold_pid_Kp
	echo 0 > ${base_path}position_sp
	echo 0 > ${base_path}speed
	echo 0 > ${base_path}speed_sp
	echo 0 > ${base_path}ramp_up_sp
	echo 0 > ${base_path}ramp_down_sp
	echo off > ${base_path}speed_regulation
	echo 0 > ${base_path}speed_pid_Kd
	echo 0 > ${base_path}speed_pid_Ki
	echo 0 > ${base_path}speed_pid_Kp
	echo running > ${base_path}state
	echo hold > ${base_path}stop_command
	echo coast brake hold > ${base_path}stop_commands
	echo 0 > ${base_path}time_sp
}

create_msensor()
{
	base_path=/tmp/libsharaku/lego-sensor/sensor${1}/
	mkdir ${base_path}

	echo ${2} > ${base_path}address
	echo  > ${base_path}bin_data
	echo  > ${base_path}bin_data_format
	echo  > ${base_path}command
	echo  > ${base_path}commands
	echo  > ${base_path}direct
	echo 1 > ${base_path}decimals
	echo  > ${base_path}driver_name
	echo 1 > ${base_path}fw_version
	echo  > ${base_path}mode
	echo  > ${base_path}modes
	echo 1 > ${base_path}num_values
	echo 4 > ${base_path}poll_ms
	echo 0 > ${base_path}units
	echo 0 > ${base_path}value0
	echo 0 > ${base_path}value1
	echo 0 > ${base_path}value2
	echo 0 > ${base_path}value3
	echo 0 > ${base_path}value4
	echo 0 > ${base_path}value5
	echo 0 > ${base_path}value6
	echo 0 > ${base_path}value7
	echo 0 > ${base_path}text_value
}

create_battery()
{
	base_path=/tmp/libsharaku/legoev3-battery/
	mkdir ${base_path}

	echo  > ${base_path}charge_full_design
	echo 5000 > ${base_path}charge_now
	echo 5000 > ${base_path}current_now
	echo 8500 > ${base_path}voltage_now
	echo 8000 > ${base_path}voltage_max_design
	echo 6000 > ${base_path}voltage_min_design
	echo  > ${base_path}status
	echo  > ${base_path}type
}

if [ ! -e /tmp/libsharaku ]; then
	mkdir /tmp/libsharaku
fi
rm -rf /tmp/libsharaku/tacho-motor
rm -rf /tmp/libsharaku/lego-sensor
rm -rf /tmp/libsharaku/legoev3-battery
mkdir /tmp/libsharaku/tacho-motor
mkdir /tmp/libsharaku/lego-sensor
mkdir /tmp/libsharaku/legoev3-battery

# motor portA
create_tacho_motor 1 outA
create_tacho_motor 2 outB
create_tacho_motor 3 outC
create_tacho_motor 4 outD
create_msensor 1 in1
create_msensor 2 in2
create_msensor 3 in3
create_msensor 4 in4
create_battery
