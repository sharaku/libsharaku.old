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

#ifndef SHARAKU_DEVICES_USONIC_EV3DEV_ULTRASONIC_H_
#define SHARAKU_DEVICES_USONIC_EV3DEV_ULTRASONIC_H_

#include <stdint.h>
#include <devices/sensor-api.hpp>
#include <devices/proc/ev3dev-proc-device.hpp>

NAMESPACE_SHARAKU_BEGIN

class ev3dev_ultrasonic
 : protected ev3dev_lego_sensor
 , public usonic_operations {
 private:
	ev3dev_ultrasonic() : ev3dev_lego_sensor(1) {}

 public:
	ev3dev_ultrasonic(const char *port, int interval_ms)
	 : ev3dev_lego_sensor(interval_ms) {
		_uint		= USONIC_MM;
		_uint_sp	= USONIC_MM;
		_dist_mm	= 0;
		_dist_in	= 0;

		connect(port);
	}
	virtual ~ev3dev_ultrasonic() {
		disconnect();
	}

	// インタフェースへのアクセス
	operator usonic_operations*() {
		return (usonic_operations*)this;
	}

 public:
	// ISonic API
	int32_t set_uint(uint u) {
		return _uint_sp = u;
	}
	int32_t	get_dist(void) {
		if (_uint == USONIC_MM) {
			return _dist_mm;
		} else {
			return _dist_in;
		}
	}

 public:
	int32_t connect(const char* port);

 protected:
	void __update(void);
	void __commit(void);
	void __io_end(PROC_IOTYPE type);

 private:
	int32_t		_dist_mm;
	int32_t		_dist_in;
	uint		_uint;
	uint		_uint_sp;
};

NAMESPACE_SHARAKU_END

#endif /* SHARAKU_DEVICES_USONIC_EV3DEV_ULTRASONIC_H_ */
