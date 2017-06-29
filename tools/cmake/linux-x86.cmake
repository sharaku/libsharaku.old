# ----------------------------------------------------------------------------
#
#  MIT License
#  
#  Copyright (c) 2016 Abe Takafumi
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
#
# platfoem: linux
# arch    : x86
#
# ----------------------------------------------------------------------------

set(CMAKE_SYSTEM_NAME Linux)

set(CROSS_PREFIX )
set(CROSS_TOOLCHAIN_PATH /usr/)
set(CROSS_FLAGS "-Wall -Wno-unused-function")

# ----------------------------------------------------------------------
# gcc静的解析
# ----------------------------------------------------------------------

# 関数がaggregate(配列や構造体など?)を返す場合に警告を出す。
set(CROSS_FLAGS "${CROSS_FLAGS} -Waggregate-return")

# # 関数呼び出しがマッチしない型にキャストされている場合に警告を出す。
# # C/ObjC
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wbad-function-cast")

# type qualifier(const,volatileなど)を外すようなポインタのキャストに警告を出す。
set(CROSS_FLAGS "${CROSS_FLAGS} -Wcast-qual")

# # 暗黙型変換のうち、表す値が変わる可能性のあるものに警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wconversion")

# 全てのコンストラクタ・デストラクタがprivateであり、かつfriendもpublic static関数も持たないクラス(=使用できないクラス)に対して警告を出す。
set(CROSS_FLAGS "${CROSS_FLAGS} -Wctor-dtor-privacy")

# __TIME__,__DATE__,__TIMESTAMP__マクロを使用している場合に警告を出す。
set(CROSS_FLAGS "${CROSS_FLAGS} -Wdate-time")

# 仮想関数を持っているのにデストラクタが仮想関数でないクラスに対して、deleteを使っている場合に警告を出す。
set(CROSS_FLAGS "${CROSS_FLAGS} -Wdelete-non-virtual-dtor")

# コードが長すぎたり複雑すぎたりして、コンパイラが最適化を実行できない場合に警告を出す。
set(CROSS_FLAGS "${CROSS_FLAGS} -Wdisabled-optimization")

# # float型が暗黙にdoubleにキャストされている場合に警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wdouble-promotion")
# # Scott Meyers の Effective C++ による次の方針に沿わない記述に警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Weffc++")
# # 浮動小数点数を==や!=で比較している場合に警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wfloat-equal")
# # -Wuninitializedが指定されている場合に、初期化されていない変数をそれ自身で初期化している場合に警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Winit-self")
# # inline指定されている関数を、コンパイラが(関数が長すぎるなどの理由で)インライン展開しなかった場合に警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Winline")
# # gotoやswitchで変数宣言を通り過ぎる場合に警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wjump-misses-init")
# # 論理演算子の間違っているかもしれない使用に対して警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wlogical-op")
# # #includeで指定されたディレクトリが見つからない場合に警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wmissing-include-dirs")
# # 複数の文字を含む文字リテラルに対して警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wmultichar")
# # Cにおいて、規格で定められた文字列長の「最小限の最大長」を超える文字列に対して警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Woverlength-strings")
# # 派生クラスの関数との名前被りによって、基底クラスのvirtual関数が使えなくなる場合に警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Woverloaded-virtual")
# # 関数型、void型に対するsizeofの適用に対して警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wpointer-arith")
# # コンストラクタのメンバ初期化子と、メンバ変数の宣言の順番が異なる場合に警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wreorder")
# # オーバーロードによってunsigned ~やenumが、signed ~に型変換される場合に警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wsign-promo")
# # -fstack-protectorが指定されている場合に、スタック保護がなされなかった関数が存在する場合に警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wstack-protector")
# # switch文がdefaultラベルの文を持たない場合に警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wswitch-default")
# # switch文の対象になる値がenumで、ラベルがenumのすべての値には対応していない場合に警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wswitch-enum")
# # ループカウンタがオーバーフローする可能性があって、ループを最適化できない場合に警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wunsafe-loop-optimizations")
# # 接尾辞のついていない浮動小数点数リテラルに対して警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wunsuffixed-float-constants")
#
# # 変数を、その変数自身の型にキャストしている(無意味である)場合に警告を出す。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wuseless-cast")

# 文字列リテラル(const char*)をchar*へ型変換する場合に警告を出す。
set(CROSS_FLAGS "${CROSS_FLAGS} -Wwrite-strings")

# # 整数リテラル'0'がヌルポインタを示す定数として使われている場合に警告を出力する。
# set(CROSS_FLAGS "${CROSS_FLAGS} -Wzero-as-null-pointer-constant")

# ----------------------------------------------------------------------

set(CROSS_FLAGS "${CROSS_FLAGS} -DSHARAKU_PROF_ENABLE -DSHARAKU_PROF_CLEAR_ENABLE")

set(CMAKE_C_COMPILER ${CROSS_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${CROSS_PREFIX}g++)
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CROSS_TOOLCHAIN_PATH}/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CROSS_TOOLCHAIN_PATH}/lib)
set(CMAKE_INSTALL_PREFIX ${CROSS_TOOLCHAIN_PATH}/)


set(TARGET_OS linux)
set(TARGET_ARCH x86)
set(TARGET_PLATFORM linux)
set(TARGET_SUFFIX "${TARGET_OS}.${TARGET_ARCH}")
