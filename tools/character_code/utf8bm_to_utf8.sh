#!/bin/sh
# ----------------------------------------------------------------------------
#
#  MIT License
#  
#  Copyright (c) 2017 Abe Takafumi
#  
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#  
#  The above copyright notice and this permission notice shall be included in all
#  copies or substantial portions of the Software.
#  
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE. *
#
# ----------------------------------------------------------------------------

readonly DEF_NKF=/usr/bin/nkf

find ../../devices -type f -name "*.c" | xargs ${DEF_NKF} -w --overwrite
find ../../devices -type f -name "*.cpp" | xargs ${DEF_NKF} -w --overwrite
find ../../devices -type f -name "*.h" | xargs ${DEF_NKF} -w --overwrite
find ../../devices -type f -name "*.hpp" | xargs ${DEF_NKF} -w --overwrite

find ../../include -type f -name "*.h" | xargs ${DEF_NKF} -w --overwrite
find ../../include -type f -name "*.hpp" | xargs ${DEF_NKF} -w --overwrite

find ../../modules -type f -name "*.c" | xargs ${DEF_NKF} -w --overwrite
find ../../modules -type f -name "*.cpp" | xargs ${DEF_NKF} -w --overwrite
find ../../modules -type f -name "*.h" | xargs ${DEF_NKF} -w --overwrite
find ../../modules -type f -name "*.hpp" | xargs  ${DEF_NKF} -w --overwrite

find ../../platform -type f -name "*.c" | xargs ${DEF_NKF} -w --overwrite
find ../../platform -type f -name "*.cpp" | xargs ${DEF_NKF} -w --overwrite
find ../../platform -type f -name "*.h" | xargs ${DEF_NKF} -w --overwrite
find ../../platform -type f -name "*.hpp" | xargs ${DEF_NKF} -w --overwrite

find ../../samples -type f -name "*.c" | xargs ${DEF_NKF} -w --overwrite
find ../../samples -type f -name "*.cpp" | xargs ${DEF_NKF} -w --overwrite
find ../../samples -type f -name "*.h" | xargs ${DEF_NKF} -w --overwrite
find ../../samples -type f -name "*.hpp" | xargs ${DEF_NKF} -w --overwrite

find ../../system -type f -name "*.c" | xargs ${DEF_NKF} -w --overwrite
find ../../system -type f -name "*.cpp" | xargs ${DEF_NKF} -w --overwrite
find ../../system -type f -name "*.h" | xargs ${DEF_NKF} -w --overwrite
find ../../system -type f -name "*.hpp" | xargs ${DEF_NKF} -w --overwrite

