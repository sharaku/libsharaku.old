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

#ifndef SHARAKU_DEVICE_CONNECTION_IF_H_
#define SHARAKU_DEVICE_CONNECTION_IF_H_

#include <stdint.h>
#include <sharaku/types.h>

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------
template<class T>
class connection_interface {
 public:
	connection_interface() {
		_interface = NULL;
	}
	void connect(T* interface) {
		_interface = interface;
	}
	void disconnect() {
		_interface = NULL;
	}
	T* operator->() {
		return _interface;
	}
	operator T*() {
		return _interface;
	}
	connection_interface<T>& operator=(T *interface) {
		connect(interface);
		return *this;
	}
 protected:
	T*	_interface;
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_DEVICE_CONNECTION_IF_H_
