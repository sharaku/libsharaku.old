/*
 * Copyright Abe Takafumi All Rights Reserved, 2015
 * Author Abe Takafumi
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

 public:
	float get_q(void) { return _q; }

 protected:
	float	_x;
	float	_q;

 private:
	low_pass_filter() {}
};


#endif // SHARAKU_UV_DIGITAL_FILTER_H_
