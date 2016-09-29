/*
 * Copyright Abe Takafumi All Rights Reserved, 2016
 * Author Abe Takafumi
 *
 */

#include <stdio.h>
#include <string.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <devices/power/hook-powersupply.hpp>

NAMESPACE_SHARAKU_BEGIN

int32_t
hook_powersupply::get_voltage(void)
{
	return out_power->get_voltage();
}

int32_t
hook_powersupply::get_voltage_max(void)
{
	return out_power->get_voltage_max();
}

int32_t
hook_powersupply::get_voltage_min(void)
{
	return out_power->get_voltage_min();
}

int32_t
hook_powersupply::get_current(void)
{
	return out_power->get_current();
}

int32_t
hook_powersupply::get_battery_type(void)
{
	return out_power->get_battery_type();
}

NAMESPACE_SHARAKU_END

