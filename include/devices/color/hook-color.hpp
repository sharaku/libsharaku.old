/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_HOOK_COLOR_H_
#define _SHARAKU_DEVICES_HOOK_COLOR_H_

#include <stdint.h>
#include <devices/sensor-api.hpp>
#include <devices/connection_interface.hpp>

NAMESPACE_SHARAKU_BEGIN

class hook_color
 : public color_operations {
 private:
	hook_color() {}
	virtual ~hook_color() {}
	// �C���^�[�t�F�[�X�ڑ�(����connect�֎~)
	connection_interface<color_operations>	out_color;

	// �C���^�t�F�[�X�ւ̃A�N�Z�X
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
