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

#ifndef SHARAKU_DEVICE_ODMETRY_API_H_
#define SHARAKU_DEVICE_ODMETRY_API_H_

#include <stdint.h>
#include <sharaku/types.h>
#include <sharaku/position.hpp>
#include <sharaku/rotation.hpp>

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------
// 座標計測インタフェース
class odmetry_operations
{
 public:
	enum uint {
		USONIC_MM,
		USONIC_IN,
	};
	virtual const position3& get_position(void) {
		static position3 pos; pos(0.0f, 0.0f, 0.0f); return pos;
	}
	virtual int32_t set_position(const position3& pos) {return -ENOTSUP;}
	virtual const rotation3& get_rotation(void) {
		static rotation3 rot; rot(0.0f, 0.0f, 0.0f); return rot;
	}
	virtual int32_t set_rotation(const rotation3& rot) {return -ENOTSUP;}
	virtual const int32_t get_dist(void) {return -ENOTSUP;}
	virtual int32_t set_dist(int32_t dist) {return -ENOTSUP;}
	virtual int32_t reset(void) {return -ENOTSUP;}
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_DEVICE_ODMETRY_API_H_
