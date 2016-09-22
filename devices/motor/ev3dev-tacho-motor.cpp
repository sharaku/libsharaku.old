/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#include <stdio.h>
#include <string.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <devices/motor/ev3dev-tacho-motor.hpp>

NAMESPACE_SHARAKU_BEGIN

int32_t ev3dev_tacho_motor::connect(const char* port)
{
	sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);

	snprintf(_profname, 64, "ev3dev_tacho_motor<%s", port);
	char *work = strstr(_profname, "\n");
	*work = '>';
	sharaku_prof_init(&_prof, _profname);
	sharaku_prof_regist(&_prof);

	int32_t result = ev3dev_lego_tacho_motor::connect(port);
	if (result != 0) {
		goto err;
	}
	// out
	ev3dev_lego_tacho_motor::duty_cycle_sp.open();
	ev3dev_lego_tacho_motor::speed_regulation.open();
	ev3dev_lego_tacho_motor::speed_sp.open();
	ev3dev_lego_tacho_motor::position_sp.open();
	ev3dev_lego_tacho_motor::duty_cycle_sp.open();
	ev3dev_lego_tacho_motor::stop_command.open();
	// in
	ev3dev_lego_tacho_motor::command.open();
	ev3dev_lego_tacho_motor::speed.open();
	ev3dev_lego_tacho_motor::duty_cycle.open();
	ev3dev_lego_tacho_motor::position.open();
	_flag = FLAG_RESET;
	sharaku_db_trace("end(0)", 0, 0, 0, 0, 0, 0);
	return 0;

 err:
	sharaku_db_trace("end(-1)", 0, 0, 0, 0, 0, 0);
	return -1;
}

// デバイスから情報を更新する
void ev3dev_tacho_motor::__update(void)
{
	_prof_time_start = sharaku_get_usec();

	// STOPもしくはRESETの場合は、その処理を行う。
	// それ以外でモード変化がある場合は変化させる。
	if (_flag & (FLAG_RESET | FLAG_STOP)) {
		if(_flag | FLAG_RESET) {
			sharaku_db_trace("motor reset", 0, 0, 0, 0, 0, 0);
			ev3dev_lego_tacho_motor::command = "reset";
			set_write_flag(DEVICE_FILE_WRITE_COMMAND);
			set_write_flag(DEVICE_FILE_WRITE_STOP_COMMAND);
			set_read_flag(DEVICE_FILE_READ_POSITION);
			set_read_flag(DEVICE_FILE_READ_COUNT_PER_ROT);
			set_read_flag(DEVICE_FILE_READ_SPEED);
			set_read_flag(DEVICE_FILE_READ_SPEED_PID_KD);
			set_read_flag(DEVICE_FILE_READ_SPEED_PID_KI);
			set_read_flag(DEVICE_FILE_READ_SPEED_PID_KP);
			set_read_flag(DEVICE_FILE_READ_HOLD_PID_KD);
			set_read_flag(DEVICE_FILE_READ_HOLD_PID_KI);
			set_read_flag(DEVICE_FILE_READ_HOLD_PID_KP);
			set_read_flag(DEVICE_FILE_READ_DUTY_CYCLE_SP);
			set_read_flag(DEVICE_FILE_READ_POSITION_SP);
			set_read_flag(DEVICE_FILE_READ_SPEED_SP);
			_speed_sp	= 0;
			_position_sp	= 0;
			_duty_cycle_sp	= 0;
			ev3dev_lego_tacho_motor::position.update();
			_position_base = ev3dev_lego_tacho_motor::position;
		} else {
			sharaku_db_trace("motor stop", 0, 0, 0, 0, 0, 0);
			ev3dev_lego_tacho_motor::command = "stop";
			set_write_flag(DEVICE_FILE_WRITE_COMMAND);
		}
		_motor_mode = MOTOR_MODE_STOP;
		_flag = _flag & ~(FLAG_RESET | FLAG_STOP);
	} else {
		if (_motor_mode_sp != _motor_mode) {
			// モードの切り替えの場合
			switch (_motor_mode_sp) {
			case MOTOR_MODE_SPEED:
				sharaku_db_trace("change mode=MOTOR_MODE_SPEED",
						 0, 0, 0, 0, 0, 0);
				ev3dev_lego_tacho_motor::speed_regulation = "on";
				set_write_flag(DEVICE_FILE_WRITE_SPEED_REGULATION);
				ev3dev_lego_tacho_motor::command = "run-forever";
				set_write_flag(DEVICE_FILE_WRITE_COMMAND);
				break;
			case MOTOR_MODE_ANGLE:
				sharaku_db_trace("change mode=MOTOR_MODE_ANGLE",
						 0, 0, 0, 0, 0, 0);
				ev3dev_lego_tacho_motor::speed_regulation = "off";
				set_write_flag(DEVICE_FILE_WRITE_SPEED_REGULATION);
				ev3dev_lego_tacho_motor::command = "run-to-abs-pos";
				set_write_flag(DEVICE_FILE_WRITE_COMMAND);
				ev3dev_lego_tacho_motor::duty_cycle_sp = 100;
				set_write_flag(DEVICE_FILE_WRITE_DUTY_CYCLE_SP);
				ev3dev_lego_tacho_motor::stop_command="hold";
				set_write_flag(DEVICE_FILE_WRITE_STOP_COMMAND);
				break;
			case MOTOR_MODE_DUTY:
				sharaku_db_trace("change mode=MOTOR_MODE_DUTY",
						 0, 0, 0, 0, 0, 0);
				ev3dev_lego_tacho_motor::speed_regulation = "off";
				set_write_flag(DEVICE_FILE_WRITE_SPEED_REGULATION);
				ev3dev_lego_tacho_motor::command = "run-direct";
				set_write_flag(DEVICE_FILE_WRITE_COMMAND);
				break;
			default:
				break;
			}
			_motor_mode = _motor_mode_sp;
		}
	}
	// モードのよって、updateする変数を変更
	// 出力側は_motor_modeによって設定。
	// 入力側は_motor_mode_spによって設定。
	switch (_motor_mode) {
	case MOTOR_MODE_SPEED:
//		if (ev3dev_lego_tacho_motor::speed_sp != _speed_sp) {
			ev3dev_lego_tacho_motor::command = "run-forever";
			set_write_flag(DEVICE_FILE_WRITE_COMMAND);
			ev3dev_lego_tacho_motor::speed_sp = _speed_sp;
			set_write_flag(DEVICE_FILE_WRITE_SPEED_SP);
			sharaku_db_trace("DEVICE_FILE_WRITE_SPEED_SP command=\"run-forever\" speed_sp=%d",
					 _speed_sp, 0, 0, 0, 0, 0);
//		}
		break;
	case MOTOR_MODE_ANGLE:
		if (ev3dev_lego_tacho_motor::position_sp != _position_sp) {
			ev3dev_lego_tacho_motor::command = "run-to-abs-pos";
			set_write_flag(DEVICE_FILE_WRITE_COMMAND);
			ev3dev_lego_tacho_motor::position_sp = _position_sp;
			set_write_flag(DEVICE_FILE_WRITE_POSITION_SP);
			sharaku_db_trace("MOTOR_MODE_ANGLE command=\"run-to-rel-pos\" position_sp=%d",
					 _position_sp, 0, 0, 0, 0, 0);
		}
		break;
	case MOTOR_MODE_DUTY:
		if (ev3dev_lego_tacho_motor::duty_cycle_sp != _duty_cycle_sp) {
			set_write_flag(DEVICE_FILE_WRITE_DUTY_CYCLE_SP);
			ev3dev_lego_tacho_motor::duty_cycle_sp = _duty_cycle_sp;
		}
		break;
	default:
		break;
	}
	switch (_motor_mode_sp) {
	case MOTOR_MODE_SPEED:
		set_read_flag(DEVICE_FILE_READ_SPEED);
		set_read_flag(DEVICE_FILE_READ_POSITION);
		break;
	case MOTOR_MODE_ANGLE:
		set_read_flag(DEVICE_FILE_READ_POSITION);
		break;
	case MOTOR_MODE_DUTY:
		set_read_flag(DEVICE_FILE_READ_DUTY_CYCLE);
		set_read_flag(DEVICE_FILE_READ_POSITION);
		break;
	default:
		break;
	}
//	if (ev3dev_lego_tacho_motor::position != _position) {
//		set_write_flag(DEVICE_FILE_WRITE_POSITION);
//		ev3dev_lego_tacho_motor::position = _position;
//	}
	// stopモードのテキストを設定
	switch (_stop_mode_sp) {
	case STOPMODE_COAST:
		ev3dev_lego_tacho_motor::stop_command="coast";
		break;
	case STOPMODE_BRAKE:
		ev3dev_lego_tacho_motor::stop_command="brake";
		break;
	case STOPMODE_HOLD:
		ev3dev_lego_tacho_motor::stop_command="hold";
		break;
	default:
		break;
	}
	if (_stop_mode != _stop_mode_sp) {
		set_write_flag(DEVICE_FILE_WRITE_STOP_COMMAND);
	}

	sharaku_db_trace("update-flag get_read_flag=0x%08x get_write_flag=0x%08x",
			 get_read_flag(), get_write_flag(), 0, 0, 0, 0);
}


// デバイスから情報を更新する
void ev3dev_tacho_motor::__io_end(void)
{
	if (_motor_mode == MOTOR_MODE_STOP) {
		// motor_ctrl_operations API
		int32_t pos = ev3dev_lego_tacho_motor::position;
		_position	= pos - _position_base;
		// speed_motor_operations API
		_count_per_rot	= ev3dev_lego_tacho_motor::count_per_rot;
		_speed		= ev3dev_lego_tacho_motor::speed;
		_speed_pid_kd	= ev3dev_lego_tacho_motor::speed_pid_Kd;
		_speed_pid_ki	= ev3dev_lego_tacho_motor::speed_pid_Ki;
		_speed_pid_kp	= ev3dev_lego_tacho_motor::speed_pid_Kp;
		// angle_motor_operations APIions API
		_hold_pid_kd	= ev3dev_lego_tacho_motor::hold_pid_Kd;
		_hold_pid_ki	= ev3dev_lego_tacho_motor::hold_pid_Ki;
		_hold_pid_kp	= ev3dev_lego_tacho_motor::hold_pid_Kp;
		// duty_motor_operations API
		_duty_cycle	= ev3dev_lego_tacho_motor::duty_cycle;
		sharaku_db_trace("motor stop _position=%d _speed=%d _duty_cycle=%d",
				 _position, _speed, _duty_cycle, 0, 0, 0);
	} else {
		switch (_motor_mode_sp) {
		case MOTOR_MODE_SPEED:
			_speed = ev3dev_lego_tacho_motor::speed;
			_position = ev3dev_lego_tacho_motor::position;
			sharaku_db_trace("motor speed _speed=%d _position=%d",
					 _speed, _position, 0, 0, 0, 0);
			break;
		case MOTOR_MODE_ANGLE:
			_position = ev3dev_lego_tacho_motor::position;
			sharaku_db_trace("motor angle _position=%d",
					 _position, 0, 0, 0, 0, 0);
			break;
		case MOTOR_MODE_DUTY:
			_duty_cycle = ev3dev_lego_tacho_motor::duty_cycle;
			_position = ev3dev_lego_tacho_motor::position;
			sharaku_db_trace("motor duty _duty_cycle=%d _position=%d",
					 _duty_cycle, _position, 0, 0, 0, 0);
			break;
		default:
			sharaku_db_trace("motor unknown type.",
					 0, 0, 0, 0, 0, 0);
			break;
		}
	}

	sharaku_usec_t time_end = sharaku_get_usec();
	sharaku_prof_add(&_prof, _prof_time_start, time_end);
}

NAMESPACE_SHARAKU_END

