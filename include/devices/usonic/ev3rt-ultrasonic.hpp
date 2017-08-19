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

#ifndef _SHARAKU_DEVICES_EV3RT_ULTRASONIC_H_
#define _SHARAKU_DEVICES_EV3RT_ULTRASONIC_H_

#include <stdint.h>
#include <sharaku/types.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <ev3api.h>
#include <devices/sensor-api.hpp>
#include <devices/ev3rt-device-def.h>

NAMESPACE_SHARAKU_BEGIN

class ev3rt_ultrasonic
 : public usonic_operations {
 private:
	ev3rt_ultrasonic() {}

 public:
	ev3rt_ultrasonic(sensor_port_t port) {
		_port = port;
		ev3_sensor_config(_port, ULTRASONIC_SENSOR);
	}
	virtual ~ev3rt_ultrasonic() {}

	// インタフェースへのアクセス
	operator usonic_operations*() {
		return (usonic_operations*)this;
	}

 public:
	virtual int32_t set_uint(usonic_operations::uint u) {
		return ENOTSUP;
	}
	virtual int32_t	get_dist(void) {
		return (int32_t)ev3_ultrasonic_sensor_get_distance(_port);
	}

 private:
	sensor_port_t _port;
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_EV3RT_ULTRASONIC_H_ */
