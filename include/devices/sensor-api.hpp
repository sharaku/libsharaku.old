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
	enum mode {
		MODE_UNKNOWN,		// 不明
		MODE_REFLECTED,		// 反射光モード
		MODE_AMBIENT,		// 環境光モード
		MODE_CORRECTION,	// 反射光 - 環境光モード
		MODE_FULLCOLOR		// 簡易カラーモード
	};
	virtual int32_t	set_mode(mode m) {return -ENOTSUP;}
	virtual mode	get_mode(void) {return MODE_UNKNOWN;}
	virtual int32_t	get_value(void) {return -ENOTSUP;}
	virtual int32_t	get_red(void) {return -ENOTSUP;}
	virtual int32_t	get_green(void) {return -ENOTSUP;}
	virtual int32_t	get_blue(void) {return -ENOTSUP;}
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
