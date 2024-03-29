﻿/* --
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


#ifndef _SHARAKU_DEVICES_HOOK_COLOR_H_
#define _SHARAKU_DEVICES_HOOK_COLOR_H_

#include <stdint.h>
#include <devices/sensor-api.hpp>

NAMESPACE_SHARAKU_BEGIN

class hook_color
 : public color_operations {
 public:
	hook_color() {}
	virtual ~hook_color() {}
	// インターフェース接続(複数connect禁止)
	color_operations	*out_color;

	// インタフェースへのアクセス
	operator color_operations*() {
		return (color_operations *)this;
	}

 public:
	virtual int32_t	set_mode(color_operations::mode m);
	virtual color_operations::mode	get_mode(void);
	virtual int32_t	get_value(void);
	virtual int32_t	get_red(void);
	virtual int32_t get_green(void);
	virtual int32_t	get_blue(void);
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_HOOK_COLOR_H_ */
