/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_HOOK_TOUCH_H_
#define _SHARAKU_DEVICES_HOOK_TOUCH_H_

#include <stdint.h>
#include <devices/sensor-api.hpp>
#include <devices/connection_interface.hpp>

NAMESPACE_SHARAKU_BEGIN

class hook_touch
 : public touch_operations {
 public:
	hook_touch() {}
	virtual ~hook_touch() {}
	// インターフェース接続(複数connect禁止)
	connection_interface<touch_operations> out_touch;

	// インタフェースへのアクセス
	operator touch_operations*() {
		return (touch_operations *)this;
	}

 public:
	virtual int32_t is_press(void);
	virtual int32_t is_push(void);
	virtual int32_t is_release(void);
};

NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_HOOK_TOUCH_H_ */
