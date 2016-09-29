/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#include <stdio.h>
#include <string.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <devices/usonic/hook-ultrasonic.hpp>

NAMESPACE_SHARAKU_BEGIN

int32_t
hook_ultrasonic::set_uint(usonic_operations::uint u)
{
	return out_usonic->set_unit(u);
}

int32_t
hook_ultrasonic::get_dist(void)
{
	return out_usonic->get_dist();
}

NAMESPACE_SHARAKU_END

