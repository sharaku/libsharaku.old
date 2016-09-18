/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_DEVICE_SENSOR_API_H_
#define SHARAKU_DEVICE_SENSOR_API_H_

#include <stdint.h>
#include <sharaku/types.h>

NAMESPACE_SHARAKU_BEGIN

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

NAMESPACE_SHARAKU_END

#endif // SHARAKU_DEVICE_SENSOR_API_H_
