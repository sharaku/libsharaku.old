/* --
 *
 * MIT License
 * 
 * Copyright (c) 2017 Abe Takafumi
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

#ifndef SHARAKU_SIMURATON_COLOR_H_
#define SHARAKU_SIMURATON_COLOR_H_

#include <stdint.h>
#include <devices/sensor-api.hpp>
#include <sharaku/image.h>

NAMESPACE_SHARAKU_BEGIN

class simuration_color
 : public color_operations
{
public:
	simuration_color();
	virtual ~simuration_color();
	operator color_operations*() {
		return (color_operations *)this;
	}

public:
	virtual int32_t	set_mode(mode m) {
		_mode = m;
		return 0;
	}
	virtual mode	get_mode(void) {
		return _mode;
	}
	virtual int32_t	get_value(void) {
		switch (_mode) {
		case MODE_REFLECTED:
			return get_reflected();
		case MODE_AMBIENT:
		case MODE_CORRECTION:
			return get_ambient();
		default:
			return -ENOTSUP;
		}
	}
	virtual int32_t	get_red(void) {
		switch (_mode) {
		case MODE_FULLCOLOR:
			return get_color().r;
		default:
			return -ENOTSUP;
		}
	}
	virtual int32_t	get_green(void) {
		switch (_mode) {
		case MODE_FULLCOLOR:
			return get_color().g;
		default:
			return -ENOTSUP;
		}
	}
	virtual int32_t	get_blue(void) {
		switch (_mode) {
		case MODE_FULLCOLOR:
			return get_color().b;
		default:
			return -ENOTSUP;
		}
	}

public:
	void set_imagefile(const char *filename);

protected:
	color_t get_color(void) {
		return _pxcel;
	}
	int32_t get_reflected(void) {
		return _reflected;
	}
	int32_t get_ambient(void) {
		return _ambient;
	}
	void set_position(int32_t x, int32_t y);

private:
	mode		_mode;

private:
	color_t		_pxcel;
	int32_t		_ambient;
	int32_t		_reflected;
	image_t		*_img;
};

NAMESPACE_SHARAKU_END

#endif
