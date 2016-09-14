/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_NULL_MOTOR_H_
#define _SHARAKU_DEVICES_NULL_MOTOR_H_

#include <stdint.h>
#include <devices/motor-api.hpp>

NAMESPACE_SHARAKU_BEGIN

class null_motor
 : virtual public motor_ctrl_operations
 , virtual public speed_motor_operations
 , virtual public angle_motor_operations
 , virtual public duty_motor_operations
{
 public:
	null_motor() {}
	virtual ~null_motor() {}

	// インタフェースへのアクセス
	operator motor_ctrl_operations*() {
		return (motor_ctrl_operations*)this;
	}
	operator speed_motor_operations*() {
		return (speed_motor_operations*)this;
	}
	operator angle_motor_operations*() {
		return (angle_motor_operations*)this;
	}
	operator duty_motor_operations*() {
		return (duty_motor_operations*)this;
	}

 public:
	virtual int32_t	get_position(void) {
		return 0;
	}
	virtual int32_t	set_position(int32_t pos) {
		return 0;
	}
	virtual int32_t	get_stop_mode(void) {
		return 0;
	}
	virtual int32_t	set_stop_mode(stop_mode mode) {
		return 0;
	}
	virtual int32_t	set_speed_pid(int kp,int ki, int kd) {
		return 0;
	}
	virtual int32_t	set_hold_pid(int kp,int ki, int kd) {
		return 0;
	}

	virtual int32_t	get_speed(void) {
		return 0;
	}
	virtual int32_t	set_speed_sp(int32_t rpm) {
		return 0;
	}
	virtual int32_t	get_speed_sp(void) {
		return 0;
	}
	virtual int32_t	get_count_per_rot(void) {
		return 0;
	}

	virtual int32_t	get_position_sp(void) {
		return 0;
	}

	virtual int32_t	set_position_sp(int32_t r) {
		return 0;
	}

	virtual int32_t	add_position_sp(int32_t r) {
		return 0;
	}


	virtual int32_t	get_duty_cycle(void) {
		return 0;
	}
	virtual int32_t	set_duty_cycle_sp(int32_t cycle) {
		return 0;
	}
	virtual int32_t	get_duty_cycle_sp(void)  {
		return 0;
	}

 protected:
	static void UpdateDevice(struct sharaku_job* job);

 public:
	int32_t connect(const char* port);
	int32_t disconnect(void);
 protected:
	static void start_update(struct sharaku_job* job);
	static void io_submit_command(struct sharaku_job* job);
	static void end_io_command(struct sharaku_job* job);
	static void io_submit_count_per_rot(struct sharaku_job* job);
	static void end_io_count_per_rot(struct sharaku_job* job);
	static void io_submit_duty_cycle(struct sharaku_job* job);
	static void end_io_duty_cycle(struct sharaku_job* job);
	static void io_submit_duty_cycle_sp(struct sharaku_job* job);
	static void end_io_duty_cycle_sp(struct sharaku_job* job);
	static void io_submit_position(struct sharaku_job* job);
	static void end_io_position(struct sharaku_job* job);
	static void io_submit_position_sp(struct sharaku_job* job);
	static void end_io_position_sp(struct sharaku_job* job);
	static void io_submit_speed(struct sharaku_job* job);
	static void end_io_speed(struct sharaku_job* job);
	static void io_submit_speed_sp(struct sharaku_job* job);
	static void end_io_speed_sp(struct sharaku_job* job);
	static void io_submit_ramp_up_sp(struct sharaku_job* job);
	static void end_io_ramp_up_sp(struct sharaku_job* job);
	static void io_submit_ramp_down_sp(struct sharaku_job* job);
	static void end_io_ramp_down_sp(struct sharaku_job* job);
	static void io_submit_speed_regulation(struct sharaku_job* job);
	static void end_io_speed_regulation(struct sharaku_job* job);
	static void io_submit_state(struct sharaku_job* job);
	static void end_io_state(struct sharaku_job* job);
	static void io_submit_stop_command(struct sharaku_job* job);
	static void end_io_stop_command(struct sharaku_job* job);
	static void io_submit_time_sp(struct sharaku_job* job);
	static void end_io_time_sp(struct sharaku_job* job);
	static void complete_update(struct sharaku_job* job);

 private:
	sharaku_job		_job;
	uint32_t		_interval_ms;
	char			_port[16];
	sharaku_usec_t		_time;

	// データ排他
	// currentを読み取り中に更新させないための排他
	sharaku_mutex_t		_mx;


	// I/O cache
	enum {
		// 入力
		UPDATE_FLAG_PORITION		= 0x00000001,
		UPDATE_FLAG_SPEED		= 0x00000002,
		UPDATE_FLAG_DUTY_CYCLE		= 0x00000004,
		UPDATE_FLAG_INPUT_MASK		= 0x0000FFFF,
		// 出力
		UPDATE_FLAG_SPEED_SP		= 0x00010000,
		UPDATE_FLAG_DUTY_CYCLE_SP	= 0x00020000,
		UPDATE_FLAG_POSITION_SP		= 0x00040000,
		UPDATE_FLAG_OUTPUT_MASK		= 0xFFFF0000,
	};
	struct Buffer {
		// 入力
		int32_t		position;
		int32_t		speed;
		int32_t		duty_cycle;
		// 出力
		int32_t		speed_sp;
		int32_t		duty_cycle_sp;
		int32_t		position_sp;
		// フラグ
		int32_t		update;
	};
	MotorMode		_motor_mode;
	StopMode		_stop_mode;

	struct Buffer		_current;
	struct Buffer		_back;
	int32_t			_base_pos;

};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_NULL_MOTOR_H_ */
