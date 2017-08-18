/* --
 *
 * MIT License
 * 
 * Copyright (c) 2014-2015 Abe Takafumi
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

#ifndef SHARAKU_DEVICES_BALANCER_H_
#define SHARAKU_DEVICES_BALANCER_H_

#include <stdint.h>
#include <sharaku/utime.h>
#include <devices/update-api.hpp>
#include <devices/moving-api.hpp>
#include <devices/sensor-api.hpp>

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------
class sd_balancer
 : public move_operations
 , public update_operations
{
 public:
	sd_balancer(float wheel_length, float max_speed);

	// インターフェース接続
	move_operations		*out_move;
	gyro_operations		*in_gyro;

	operator move_operations*() { return (move_operations*)this;}
	operator update_operations*() { return (update_operations*)this;}

 public:
	// 設定値
	float		gain_gyroangle;	///< 角度Gain
	float		gain_gyrospeed;	///< 角速度Gain
	float		gain_pos;	///< 距離Gain
	float		gain_drive;	///< 走行Gain
	float		gain_speed;	///< 速度Gain
	float		wheel_ratio;	///< 車輪係数

 public:
	// sm_move_op インタフェース
	virtual void balancer_on(void) {
		_onoff = true;
	}
	virtual void balancer_off(void) {
		_onoff = false;
	}
	// sm_move_op インタフェース
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

	// update_operations インタフェース
	virtual int32_t update(const float &interval, uint32_t retry_cnt);

 private:
	sd_balancer() {}
	sharaku_usec_t		_time;		// 前回実行時の時間
	bool			_onoff;		// 機能On/Off

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
