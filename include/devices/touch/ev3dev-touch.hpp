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

#ifndef SHARAKU_UV_DEVICE_EV3DEV_TOUCH_H_
#define SHARAKU_UV_DEVICE_EV3DEV_TOUCH_H_

#include <stdint.h>
#include <sharaku/log.h>
#include <devices/sensor-api.hpp>
#include <devices/proc/ev3dev-proc-device.hpp>

NAMESPACE_SHARAKU_BEGIN

class ev3dev_touch
 : protected ev3dev_lego_sensor
 , public touch_operations {
 private:
	ev3dev_touch() : ev3dev_lego_sensor(1) {}

 public:
	ev3dev_touch(const char *port, int interval_ms)
	 : ev3dev_lego_sensor(interval_ms) {
		sharaku_db_trace("start", 0, 0, 0, 0, 0, 0);
		_press		= 0;
		_push		= 0;
		_release	= 0;

		ev3dev_touch::connect(port);
		sharaku_db_trace("end", 0, 0, 0, 0, 0, 0);
	}
	virtual ~ev3dev_touch() {
		disconnect();
	}
	operator touch_operations*() {
		return (touch_operations *)this;
	}

 public:
	int32_t is_press(void) {
		return _press;
	}
	int32_t is_push(void) {
		// 一度回収したら、状態をクリアする。
		// これを行わないと、APIを使う側で1を取り出せない
		int32_t ret = _push;
		_push = 0;
		return ret;
	}
	int32_t is_release(void) {
		// 一度回収したら、状態をクリアする。
		// これを行わないと、APIを使う側で1を取り出せない
		int32_t ret = _release;
		_release = 0;
		return ret;
	}

 public:
	int32_t connect(const char* port);

 protected:
	void __update(void);
	void __commit(void);
	void __io_end(PROC_IOTYPE type);

 private:
	int32_t		_press;
	int32_t		_push;
	int32_t		_release;
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_UV_DEVICE_EV3DEV_LARGE_MOTOR_H_
