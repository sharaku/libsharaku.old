/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#include <stdio.h>
#include <string.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <devices/touch/hook-touch.hpp>

NAMESPACE_SHARAKU_BEGIN

int32_t
hook_touch::is_press(void)
{
	return out_touch->is_press();
}

int32_t
hook_touch::is_push(void)
{
	return out_touch->is_push();
}

int32_t
hook_touch::is_release(void)
{
	return out_touch->is_release();
}

NAMESPACE_SHARAKU_END

