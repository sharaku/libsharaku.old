/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
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
