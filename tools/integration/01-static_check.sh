#!/bin/sh

# cppcheckを使用してコードチェックを行う
rm -rf result
mkdir -p result
cppcheck --enable=all --xml ../../ 2> ./result/result-cppcheck.xml
