/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_UV_API_H_
#define SHARAKU_UV_API_H_

#include <stdint.h>
#include <sharaku/types.h>
#include <sharaku/prof.h>
#include <sharaku/position.hpp>
#include <sharaku/rotation.hpp>
#include <sharaku/vector.hpp>

#define NAMESPACE_SHARAKU_BEGIN	namespace sharaku {
#define NAMESPACE_SHARAKU_END	};

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------
// updateインタフェース

class update_operations
{
 public:
	update_operations() {
		INIT_LIST_HEAD(&update_list);
	}
	virtual void	pre_update(const float &interval) {}
	virtual void	update(const float &interval) {}
	virtual void	post_update(const float &interval) {}
 public:
	list_head	update_list;
};

//-----------------------------------------------------------------------------
// 電流/電圧インタフェース

class powersupply_operations
{
 public:
	virtual int32_t	get_voltage(void) {return -ENOTSUP;}
	virtual int32_t	get_voltage_max(void) {return -ENOTSUP;}
	virtual int32_t	get_voltage_min(void) {return -ENOTSUP;}
	virtual int32_t	get_current(void) {return -ENOTSUP;}
	virtual int32_t	get_battery_type(void) {return -ENOTSUP;}
};

//-----------------------------------------------------------------------------
// ジャイロセンサインタフェース

class gyro_operations
{
 public:
	virtual int32_t	get_angle(void) {return -ENOTSUP;}
	virtual int32_t	get_rate(void) {return -ENOTSUP;}
	virtual int32_t	reset(void) {return -ENOTSUP;}
};

//-----------------------------------------------------------------------------
// カラーセンサインタフェース

class color_operations
{
 public:
	virtual int32_t	get_red(void) {return -ENOTSUP;}
	virtual int32_t	get_green(void) {return -ENOTSUP;}
	virtual int32_t	get_blue(void) {return -ENOTSUP;}
};

//-----------------------------------------------------------------------------
// 光センサインタフェース

class light_operations
{
 public:
	enum color {
		COLOR_NONE,
		COLOR_BLACK,
		COLOR_BLUE,
		COLOR_GREEN,
		COLOR_YELLOW,
		COLOR_RED,
		COLOR_WHITE,
		COLOR_BROWN,
	};
	enum mode {
		MODE_UNKNOWN,		// 不明
		MODE_COLOR,		// 簡易カラーモード
		MODE_REFLECTED,		// 反射光モード
		MODE_AMBIENT,		// 環境光モード
		MODE_CORRECTION		// 反射光 - 環境光モード
	};
	virtual mode	get_mode(void) {return MODE_UNKNOWN;}
	virtual int32_t	get_value(void) {return -ENOTSUP;}
};

//-----------------------------------------------------------------------------
// 超音波センサインタフェース

class usonic_operations
{
 public:
	enum uint {
		USONIC_MM,
		USONIC_IN,
	};
	virtual int32_t	set_unit(uint) {return -ENOTSUP;}
	virtual int32_t	get_dist(void) {return -ENOTSUP;}
};

//-----------------------------------------------------------------------------
// 温度センサインタフェース

class thermostat_operations
{
 public:
	enum uint {
		THERMOSTAT_C,
		THERMOSTAT_F,
	};
	virtual int32_t	set_unit(uint) {return -ENOTSUP;}
	virtual int32_t	get_degree(void) {return -ENOTSUP;}
};

//-----------------------------------------------------------------------------
// タッチセンサ/釦インタフェース

class touch_operations
{
 public:
	virtual int32_t	is_press(void) {return false;}
	virtual int32_t	is_push(void) {return false;}
	virtual int32_t	is_release(void) {return false;}
};

//-----------------------------------------------------------------------------
// モータの制御インタフェース

class motor_ctrl_operations
{
 public:
	enum stop_mode {
		STOPMODE_COAST,	// 電力をカットし、自然に止まる（惰性）
		STOPMODE_BRAKE,	// ブレーキをかけつつ、ゆっくり止まる
		STOPMODE_HOLD,	// 現在の位置を維持しようとする
		STOPMODE_UNKNOWN,	// ????
	};

	virtual int32_t	get_position(void) {return -ENOTSUP;}
	virtual int32_t	set_position(int32_t pos) {return -ENOTSUP;}
	virtual int32_t	get_stop_mode(void) {return -ENOTSUP;}
	virtual int32_t	set_stop_mode(stop_mode mode) {return -ENOTSUP;}
	virtual int32_t	reset(void) {return -ENOTSUP;}
	virtual int32_t	stop(void) {return -ENOTSUP;}
};

//-----------------------------------------------------------------------------
// 速度指定のモータインタフェース

class speed_motor_operations
 : virtual public motor_ctrl_operations
{
 public:
	virtual int32_t	get_speed(void) {return -ENOTSUP;}
	virtual int32_t	set_speed_sp(int32_t dps) {return -ENOTSUP;}
	virtual int32_t	get_speed_sp(void) {return -ENOTSUP;}
	virtual int32_t	get_count_per_rot(void) {return -ENOTSUP;}
	virtual int32_t	set_speed_pid(int kp,int ki, int kd) {return -ENOTSUP;}
};

//-----------------------------------------------------------------------------
// 角度指定のモータインタフェース

class angle_motor_operations
 : virtual public motor_ctrl_operations
{
 public:
	virtual int32_t	get_position_sp(void) {return -ENOTSUP;}
	virtual int32_t	set_position_sp(int32_t r) {return -ENOTSUP;}
	virtual int32_t	add_position_sp(int32_t r) {return -ENOTSUP;}
	virtual int32_t	set_hold_pid(int kp,int ki, int kd) {return -ENOTSUP;}
};

//-----------------------------------------------------------------------------
// パワー指定のモータインタフェース

class duty_motor_operations
 : virtual public motor_ctrl_operations
{
 public:
	virtual int32_t	get_duty_cycle(void) {return -ENOTSUP;}
	virtual int32_t	set_duty_cycle_sp(int32_t cycle) {return -ENOTSUP;}
	virtual int32_t	get_duty_cycle_sp(void) {return -ENOTSUP;}
};

//-----------------------------------------------------------------------------
// 車両の走行をspeedとsteeringで表現するインタフェース

class move_operations
{
 public:
	virtual int32_t	get_steer_sp(void) {return -ENOTSUP;}
	virtual int32_t	get_speed_sp(void) {return -ENOTSUP;}
	virtual int32_t	set_speed_sp(int32_t dps) {return -ENOTSUP;}
	virtual int32_t	set_steer_sp(int32_t steer_deg) {return -ENOTSUP;}
	virtual int32_t	get_speed(void) {return -ENOTSUP;}
	virtual int32_t	get_steer_deg(void) {return -ENOTSUP;}
	virtual int32_t	get_steer_rho(void) {return -ENOTSUP;}
	virtual int32_t	get_position(void) {return -ENOTSUP;}
};

//-----------------------------------------------------------------------------
// 座標計測インタフェース
class odmetry_operations
{
 public:
	enum uint {
		USONIC_MM,
		USONIC_IN,
	};
	virtual const position3& get_position(void) = 0;
	virtual const rotation3& get_rotation(void) = 0;
	virtual const int32_t get_dist(void) = 0;
	virtual int32_t reset(void) {return -ENOTSUP;}
};

//-----------------------------------------------------------------------------
// ターゲット座標インタフェース
class position_move_operations
{
 public:
	virtual int32_t	set_position_sp(position3&) {return -ENOTSUP;}
	virtual const position3& get_position_sp(void) = 0;
	virtual int32_t	set_rotation_sp(rotation3&) {return -ENOTSUP;}
	virtual const rotation3& get_rotation_sp(void) = 0;
};

//-----------------------------------------------------------------------------
template<class T>
class connection_interface {
 public:
	connection_interface() {
		_interface = NULL;
	}
	void connect(T* interface) {
		_interface = interface;
	}
	void disconnect() {
		_interface = NULL;
	}
	T* operator->() {
		return _interface;
	}
	operator T*() {
		return _interface;
	}
	connection_interface<T>& operator=(T *interface) {
		connect(interface);
		return *this;
	}
 protected:
	T*	_interface;
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_UV_API_H_
