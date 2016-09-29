/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#include <stdio.h>
#include <string.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <devices/gyro/hook-gyro.hpp>

NAMESPACE_SHARAKU_BEGIN

int32_t
hook_gyro::get_angle(void)
{
	return out_gyro->get_angle();
}


int32_t
hook_gyro::get_rate(void)
{
	return out_gyro->get_rate();
}


int32_t
hook_gyro::reset(void)
{
	return out_gyro->reset();
}


NAMESPACE_SHARAKU_END

