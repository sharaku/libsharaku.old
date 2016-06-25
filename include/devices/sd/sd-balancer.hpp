/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2015
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_DEVICES_BALANCER_H_
#define SHARAKU_DEVICES_BALANCER_H_

#include <stdint.h>
#include <devices/api.hpp>

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------
class sd_balancer
 : public move_operations
 , public update_operations
{
 public:
	sd_balancer(float wheel_length, float max_speed);

	// �C���^�[�t�F�[�X�ڑ�
	connection_interface<move_operations>	out_move;
	connection_interface<gyro_operations>	in_gyro;

	operator move_operations*() { return (move_operations*)this;}
	operator update_operations*() { return (update_operations*)this;}

 public:
	// �ݒ�l
	float		gain_gyroangle;	///< �p�xGain
	float		gain_gyrospeed;	///< �p���xGain
	float		gain_pos;	///< ����Gain
	float		gain_drive;	///< ���sGain
	float		gain_speed;	///< ���xGain
	float		wheel_ratio;	///< �ԗ֌W��

 public:
	// sm_move_op �C���^�t�F�[�X
	virtual void balancer_on(void) {
		_onoff = true;
	}
	virtual void balancer_off(void) {
		_onoff = false;
	}
	// sm_move_op �C���^�t�F�[�X
	virtual int32_t	get_steer_sp(void) {
		return out_move->get_steer_sp();
	}
	virtual int32_t	get_speed_sp(void) {
		return out_move->get_steer_sp();
	}
	virtual int32_t	set_speed_sp(int32_t dps);
	virtual int32_t	set_steer_sp(int32_t steer);
	virtual int32_t	get_speed(void) {
		return out_move->get_speed();
	}
	virtual int32_t	get_steer_deg(void) {
		return out_move->get_steer_deg();
	}
	virtual int32_t	get_position(void) {
		return out_move->get_position();
	}

	// update_operations �C���^�t�F�[�X
	virtual void update(const float &interval);

 private:
	sd_balancer() {}
	sharaku_usec_t		_time;		// �O����s���̎���
	bool			_onoff;		// �@�\On/Off

 private:
	int32_t			_speed;
	float			_max_speed;
	int32_t			_power;
	int32_t			_steering;
	float			_motorPos;
	float			_motorControlDrive;
	float			_rho;
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_UV_MODULES_LINETRACE_H_
