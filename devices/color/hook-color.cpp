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

