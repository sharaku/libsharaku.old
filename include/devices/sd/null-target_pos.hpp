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

#ifndef _SHARAKU_DEVICES_NULL_ODOMETRY_H_
#define _SHARAKU_DEVICES_NULL_ODOMETRY_H_

#include <stdint.h>
#include <devices/moving-api.hpp>

NAMESPACE_SHARAKU_BEGIN

class null_target_pos
 : public target_pos_operations {
 public:
	null_target_pos() {
		pos(0.0f, 0.0f, 0.0f)
		rot(0.0f, 0.0f, 0.0f)
	}
	virtual ~null_target_pos(){}
	operator target_pos_operations*() {
		return (target_pos_operations*)this;
	}

 public:
	virtual int32_t	set_position_sp(position3&) {
		return 0;
	}
	virtual const position3& get_position_sp(void) {
		return pos;
	}
	virtual int32_t	set_rotation_sp(rotation3&) {
		return 0;
	}
	virtual const rotation3& get_rotation_sp(void){
		return rot;
	}

 private:
	position3	pos;
	rotation3	rot;
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_NULL_ODOMETRY_H_ */
