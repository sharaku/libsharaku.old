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

#ifndef _SHARAKU_DEVICES_HOOK_POWERSUPPLY_H_
#define _SHARAKU_DEVICES_HOOK_POWERSUPPLY_H_

#include <stdint.h>
#include <devices/sensor-api.hpp>

NAMESPACE_SHARAKU_BEGIN

class hook_powersupply
 : public powersupply_operations {
 public:
	hook_powersupply() {}
	virtual ~hook_powersupply(){}
	// インターフェース接続(複数connect禁止)
	powersupply_operations		*out_power;

	// インタフェースへのアクセス
	operator powersupply_operations*() {
		return (powersupply_operations*)this;
	}

 public:
	virtual int32_t	get_voltage(void);
	virtual int32_t	get_voltage_max(void);
	virtual int32_t	get_voltage_min(void);
	virtual int32_t	get_current(void);
	virtual int32_t	get_battery_type(void);
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_HOOK_POWERSUPPLY_H_ */
