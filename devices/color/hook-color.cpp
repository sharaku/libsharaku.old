/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#include <stdio.h>
#include <string.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <devices/color/hook-color.hpp>

NAMESPACE_SHARAKU_BEGIN

int32_t
hook_color::set_mode(color_operations::mode m)
{
	return out_color->set_mode(m);
}


color_operations::mode
hook_color::get_mode(void)
{
	return out_color->get_mode();
}


int32_t
hook_color::get_value(void)
{
	return out_color->get_value();
}


int32_t
hook_color::get_red(void)
{
	return out_color->get_red();
}


int32_t
hook_color::get_green(void)
{
	return out_color->get_green();
}


int32_t
hook_color::get_blue(void)
{
	return out_color->get_blue();
}


NAMESPACE_SHARAKU_END

