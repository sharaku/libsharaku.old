/* --
 *
 * MIT License
 * 
 * Copyright (c) 2015 Abe Takafumi
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

#ifndef SHARAKU_UV_DIGITAL_FILTER_H_
#define SHARAKU_UV_DIGITAL_FILTER_H_

//-----------------------------------------------------------------------------
// １次ローパスフィルタ（Low-pass filter: LPF）の実装
class low_pass_filter
{
 public:
	low_pass_filter(float q) {
		set(q);
		clear();
	}
	void clear(void) {
		_x = 0;
	}
	void set(float q) {
		_q = q;
	}
	float operator+(float x) {
		_x = (x * _q) + (_x * (1 - _q));
		return _x;
	}
	low_pass_filter& operator+=(float x) {
		_x = (x * _q) + (_x * (1 - _q));
		return *this;
	}
	low_pass_filter& operator=(float x) {
		_x = x;
		return *this;
	}
	operator float() {
		return _x;
	}

 public:
	float get_q(void) { return _q; }

 protected:
	float	_x;
	float	_q;

 private:
	low_pass_filter() {}
};


#endif // SHARAKU_UV_DIGITAL_FILTER_H_
