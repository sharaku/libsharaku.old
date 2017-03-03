/* --
 *
 * MIT License
 * 
 * Copyright (c) 2016 Abe Takafumi
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
 * SOFTWARE.
 *
 */

#include <stdio.h>
#include <string.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <devices/motor/hook-motor.hpp>

NAMESPACE_SHARAKU_BEGIN

int32_t
hook_motor::get_position(void)
{
	if (out_speed) {
		return out_speed->get_position();
	} else if (out_duty) {
		return out_duty->get_position();
	} else {
		return out_angle->get_position();
	}
}

int32_t
hook_motor::set_position(int32_t pos)
{
	if (out_speed) {
		return out_speed->set_position(pos);
	} else if (out_duty) {
		return out_duty->set_position(pos);
	} else {
		return out_angle->set_position(pos);
	}
}

int32_t
hook_motor::get_stop_mode(void)
{
	if (out_speed) {
		return out_speed->get_stop_mode();
	} else if (out_duty) {
		return out_duty->get_stop_mode();
	} else {
		return out_angle->get_stop_mode();
	}
}

int32_t
hook_motor::set_stop_mode(stop_mode mode)
{
	if (out_speed) {
		return out_speed->set_stop_mode(mode);
	} else if (out_duty) {
		return out_duty->set_stop_mode(mode);
	} else {
		return out_angle->set_stop_mode(mode);
	}
}


int32_t
hook_motor::get_speed(void)
{
	return out_speed->get_speed();
}

int32_t
hook_motor::set_speed_sp(int32_t rpm)
{
	return out_speed->set_speed_sp(rpm);
}

int32_t
hook_motor::get_speed_sp(void)
{
	return out_speed->get_speed_sp();
}

int32_t
hook_motor::get_count_per_rot(void)
{
	return out_speed->get_count_per_rot();
}

int32_t
hook_motor::set_speed_pid(float kp,float ki, float kd)
{
	return out_speed->set_speed_pid(kp, ki, kd);
}


int32_t
hook_motor::get_position_sp(void)
{
	return out_angle->get_position_sp();
}

int32_t
hook_motor::set_position_sp(int32_t r)
{
	return out_angle->set_position_sp(r);
}

int32_t
hook_motor::add_position_sp(int32_t r)
{
	return out_angle->add_position_sp(r);
}

int32_t
hook_motor::set_hold_pid(float kp,float ki, float kd)
{
	return out_angle->set_hold_pid(kp, ki, kd);
}



int32_t
hook_motor::get_duty_cycle(void)
{
	return out_duty->get_duty_cycle();
}

int32_t
hook_motor::set_duty_cycle_sp(int32_t cycle)
{
	return out_duty->set_duty_cycle_sp(cycle);
}

int32_t
hook_motor::get_duty_cycle_sp(void)
{
	return out_duty->get_duty_cycle_sp();
}


NAMESPACE_SHARAKU_END

