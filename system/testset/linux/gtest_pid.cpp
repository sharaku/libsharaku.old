/* --
 *
 * MIT License
 * 
 * Copyright (c) 2017 Abe Takafumi
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

#include <sharaku/pid.hpp>
#include <gtest/gtest.h>

TEST(pid, pid) {
	pid	p(1.0f, 1.1f, 1.2f);

	EXPECT_EQ(p.get_Kp(), 1.0f);
	EXPECT_EQ(p.get_Ki(), 1.1f);
	EXPECT_EQ(p.get_Kd(), 1.2f);
	EXPECT_EQ(p.get_ei(), 0.0f);
	EXPECT_EQ(p.get_el(), 0.0f);
}

TEST(pid, clear) {
	pid	p(1.0f, 1.1f, 1.2f);

	p(0.01, 0.02, 1000);
	EXPECT_NE(p.get_ei(), 0.0f);
	EXPECT_NE(p.get_el(), 0.0f);

	p.clear();
	EXPECT_EQ(p.get_ei(), 0.0f);
	EXPECT_EQ(p.get_el(), 0.0f);
}

TEST(pid, set_pid) {
	pid	p(1.0f, 1.1f, 1.2f);
	p.set_pid(2.0f, 2.1f, 2.2f);

	EXPECT_EQ(p.get_Kp(), 2.0f);
	EXPECT_EQ(p.get_Ki(), 2.1f);
	EXPECT_EQ(p.get_Kd(), 2.2f);
}

TEST(pid, operator_func) {
}

TEST(pid, get_Kp) {
	pid	p(1.0f, 1.1f, 1.2f);

	EXPECT_EQ(p.get_Kp(), 1.0f);
}

TEST(pid, get_Ki) {
	pid	p(1.0f, 1.1f, 1.2f);

	EXPECT_EQ(p.get_Ki(), 1.1f);
}

TEST(pid, get_Kd) {
	pid	p(1.0f, 1.1f, 1.2f);

	EXPECT_EQ(p.get_Kd(), 1.2f);
}
