/*****************************************************************************/
/*!
@author	abe takafumi
@par	Copyright
2014-2015 abe takafumi All Rights Reserved.
@file	
@brief	2�̃��[�^���g�p���ē|����������
******************************************************************************/

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <devices/sd/sd-balancer.hpp>
#include <sharaku/log.h>

#define PROFILE

NAMESPACE_SHARAKU_BEGIN

/******************************************************************************
definition section
******************************************************************************/
#define WHEEL_RATIO_NXT1	1.0f // 56mm
#define WHEEL_RATIO_NXT		0.8f // 43.2mm (same as EV3)
#define WHEEL_RATIO_RCX		1.4f // 81.6mm

// This constant is in degrees/second for maximum speed.  Note that
// position and speed are measured as the sum of the two motors, in
// other words, 600 would actually be 300 degrees/second for each
// motor.
#define CONTROL_SPEED  600.0f

static float
speed2duty(int32_t speed, float max_speed)
{
	return speed / max_speed * 100.0f;
}

static float
duty2speed(int32_t duty, float max_speed)
{
	return max_speed * (duty / 100.0f);
}

/******************************************************************************
class section
******************************************************************************/
sd_balancer::sd_balancer(float wheel_length, float max_speed)
{
	sharaku_db_trace("", 0, 0, 0, 0, 0, 0);
	_time = 0;
	_onoff = false;

	gain_gyroangle	= 7.5;
	gain_gyrospeed	= 1.15;
	gain_pos	= 0.07;
	gain_speed	= 0.1;
	gain_drive	= -0.02;
	wheel_ratio	= wheel_length / 56.0;

	_speed			= 0;
	_max_speed		= fabs(max_speed);
	_steering		= 0;
	_motorPos		= 0;
	_motorControlDrive	= 0;
}

/******************************************************************************
IMove
******************************************************************************/

// power = -200 �` 200 (+-200�͈��肵�Ȃ�)
int32_t sd_balancer::set_speed_sp(int32_t speed)
{
	_speed = speed;
	register float	power = speed2duty(speed, _max_speed);

	// �Ȃ����������t�B
	// ToDo: �Ȃ��t�Ȃ̂��B
	sharaku_db_trace("_motorControlDrive = (%d * 2) * CONTROL_SPEED / 200.0", power, 0, 0, 0, 0, 0);
	_motorControlDrive = (-1 * power * CONTROL_SPEED) / (100.0 * 2);

	return speed;
}

int32_t sd_balancer::set_steer_sp(int32_t steer)
{
	_steering	= steer;
	return _steering;
}


void sd_balancer::update(const float &interval)
{
	sharaku_db_trace("interval=%d", (int32_t)(interval * 1000.0f), 0, 0, 0, 0, 0);

	// ���Ԏ��W(�ŏ���1��͍̎悵�Ȃ�)
	sharaku_usec_t time = sharaku_get_usec();
	if (_time) {
	}
	_time = time;

	register int32_t gyro_speed	= in_gyro->get_rate();
	register int32_t gyro_angle	= in_gyro->get_angle();
	register int32_t motors_speed	= out_move->get_speed();
	register int32_t motors_pos	= out_move->get_position();
	register float power;

	if (!_onoff) {
		goto balanser_off;
	}

	// ���x�����Z
	_motorPos += (float)_motorControlDrive * interval;
	sharaku_db_trace("_motorPos(%d) += %d * %d", (int32_t)_motorPos, _motorControlDrive, interval * 1000.0f, 0, 0, 0);

	// �����œ���������������
	motors_pos = motors_pos - (int32_t)_motorPos;

	sharaku_db_trace("gyro_speed = %d, gyro_angle = %d", gyro_speed, gyro_angle, 0, 0, 0, 0);
	sharaku_db_trace("motors_pos = %d, motors_speed = %d, motorControlDrive = %d", motors_pos, motors_speed, _motorControlDrive, 0, 0, 0);
	power =		(gain_gyrospeed * (float)gyro_speed
				 + (float)gain_gyroangle * (float)gyro_angle)
				 / (float)wheel_ratio
			+ gain_pos * motors_pos
			+ gain_drive * _motorControlDrive
			+ gain_speed * (float)motors_speed;

	power = (power > 100.0) ? 100.0 : power;
	power = (power < -100.0) ? -100.0 : power;

	sharaku_db_trace("power = %d", (int32_t)(power), 0, 0, 0, 0, 0);

	out_move->set_speed_sp((int32_t)(duty2speed(power, _max_speed)));
	out_move->set_steer_sp(_steering);

	// ���Ԏ��W
	time = sharaku_get_usec();
	sharaku_db_trace("time=%d", (int32_t)(time - _time), 0, 0, 0, 0, 0);
	return;

 balanser_off:
	// �o�����TOFF�̏ꍇ�͎w�肳��Ă���Speed/Steer�����̂܂܃X���[����
	out_move->set_speed_sp(_speed);
	out_move->set_steer_sp(_steering);
	return;
}

NAMESPACE_SHARAKU_END

