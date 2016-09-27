#!/bin/sh

# cppcheckを使用してコードチェックを行う
mkdir -p result
rm -f result-cppcheck.xml
cd ../../
cppcheck --enable=all --xml . 2> ./tools/integration/result/result-cppcheck.xml
