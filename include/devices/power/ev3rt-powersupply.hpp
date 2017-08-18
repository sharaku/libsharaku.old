/* --
 *
 * MIT License
 * 
 * Copyright (c) 2016 Abe Takafumi
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

#ifndef _SHARAKU_DEVICES_EV3RT_POWERSUPPLY_H_
#define _SHARAKU_DEVICES_EV3RT_POWERSUPPLY_H_

#include <stdint.h>
#include <sharaku/types.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <ev3api.h>
#include <devices/sensor-api.hpp>

NAMESPACE_SHARAKU_BEGIN

class ev3rt_powersupply
 : public powersupply_operations {
 public:
	ev3rt_powersupply() {}
	virtual ~ev3rt_powersupply(){}

	operator powersupply_operations*() {
		return (powersupply_operations*)this;
	}

 public:
	virtual int32_t	get_voltage(void) {
		return ev3_battery_voltage_mV();
	}
	virtual int32_t	get_voltage_max(void) {
		return 10000;
	}
	virtual int32_t	get_voltage_min(void) {
		return 6000;
	}
	virtual int32_t	get_current(void) {
		return ev3_battery_current_mA();
	}
	virtual int32_t	get_battery_type(void) {
		return ENOTSUP;
	}
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_HOOK_POWERSUPPLY_H_ */
