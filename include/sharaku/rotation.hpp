/* --
 *
 * MIT License
 * 
 * Copyright (c) 2004-2016 Abe Takafumi
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
 * SOFTWARE. *
 *
 */

#ifndef SHARAKU_UTILTY_ROTATION_H_
#define SHARAKU_UTILTY_ROTATION_H_

#include <math.h>
#include <sharaku/vector.hpp>

// 数学定義
#ifndef M_PI
#define M_PI	(3.14159265358979323846264332795f)
#endif
// 数学定義
#ifndef M_PI_180
#define M_PI_180	(3.14159265358979323846264332795f / 180.0f)
#endif

// 半径をSteering角度へ変換
static float
sharaku_rho2steering(int32_t rho, int32_t wheel_length)
{
	register float tan_theta = ((float)wheel_length * M_PI) / rho;
	int32_t steering = (int32_t)(atan(tan_theta) / M_PI_180);
	return steering;
}

// Steering角度を半径へ変換
static float
sharaku_steering2rho(int32_t steering, int32_t wheel_length)
{
	register float tan_theta = tan((float)steering * M_PI_180);
	register float rho = 0.0f;
	rho = ((float)wheel_length * M_PI) / tan_theta;
	return rho;
}

// 角度の差分を取得する。leftrightは旋回方向であり、-1, 0, +1を指定する
static int32_t
sharaku_differ_degree(int32_t target, int32_t now, int leftright)
{
	// 起点Xを0点とした差分を作成する
	int32_t result;
	if (leftright > 0) {
		// 左回りでの角度差分をとる
		int32_t diff = 360 + target - now;
		if (diff >= 360) {
			result = diff - 360;
		} else {
			result = diff;
		}
	} else if (leftright < 0) {
		// 右回りでの角度差分をとる
		int32_t diff = -360 + target - now;
		if (diff <= 360) {
			result = diff + 360;
		} else {
			result = diff;
		}
	} else {
		// 近い方を選択する
		int32_t diff1 = sharaku_differ_degree(target, now, 1);
		int32_t diff2 = sharaku_differ_degree(target, now, -1);
		if (fabs(diff2) > diff1) {
			result = diff1;
		} else {
			result = diff2;
		}
	}
	return result;
}

/* ========================================================================= */
/* struct definition Section                                                 */
/* ========================================================================= */

/*! @class rotation3
    @brief  3次元ベクトル(移動量)構造体
*/
struct rotation3 {
 public:
	float x; ///< x軸の回転角
	float y; ///< y軸の回転角
	float z; ///< z軸の回転角

 public:
	/*********************************************************************/
	/*! @brief x, y, zを代入する

		@param[in]      rotat_x           新規設定を行うX軸の回転角度
		@param[in]      rotat_y           新規設定を行うY軸の回転角度
		@param[in]      rotat_z           新規設定を行うZ軸の回転角度(省略時は0.0fとなる)
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         設定後の3次元回転角度構造体
		@exception      none
	**********************************************************************/
	rotation3& operator()(float rotat_x, float rotat_y, float rotat_z = 0.0f) {
		x = rotat_x; y = rotat_y; z = rotat_z;
		return (*this);
	}

	/*********************************************************************/
	/*! @brief rotation3構造体を代入する

		@param[in]      rotat             代入する構造体
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         設定後の3次元回転角度構造体
		@exception      none
	**********************************************************************/
	rotation3& operator=(rotation3& rotat) {
		x = rotat.x; y = rotat.y; z = rotat.z;
		return (*this);
	}
	rotation3& operator=(const rotation3& rotat) {
		x = rotat.x; y = rotat.y; z = rotat.z;
		return (*this);
	}

	/*********************************************************************/
	/*! @brief 各座標ごとに加算を行う

		@param[in]      rotat             加算するrotation3
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         加算結果
		@exception      none
	**********************************************************************/
	rotation3 operator+(rotation3& rotat) {
		rotation3 result;
		result.x = x + rotat.x; result.y = y + rotat.y; result.z = z + rotat.z;
		return result;
	}
	rotation3 operator+(const rotation3& rotat) {
		rotation3 result;
		result.x = x + rotat.x; result.y = y + rotat.y; result.z = z + rotat.z;
		return result;
	}

	/*********************************************************************/
	/*! @brief 各座標ごとに減算を行う

		@param[in]      rotat             減算するrotation3
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         減算結果
		@exception      none
	**********************************************************************/
	rotation3 operator-(rotation3& rotat) {
		rotation3 result;
		result.x = x - rotat.x; result.y = y - rotat.y; result.z = z - rotat.z;
		return result;
	}
	rotation3 operator-(const rotation3& rotat) {
		rotation3 result;
		result.x = x - rotat.x; result.y = y - rotat.y; result.z = z - rotat.z;
		return result;
	}

	/*********************************************************************/
	/*! @brief 各座標ごとに加算を行う

		@param[in]      vec             加算するvector3
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         加算結果
		@exception      none
	**********************************************************************/
	rotation3 operator+(vector3& vec) {
		rotation3 result;
		result.x = x + vec.x; result.y = y + vec.y; result.z = z + vec.z;
		return result;
	}
	rotation3 operator+(const vector3& vec) {
		rotation3 result;
		result.x = x + vec.x; result.y = y + vec.y; result.z = z + vec.z;
		return result;
	}

	/*********************************************************************/
	/*! @brief 各座標ごとに減算を行う

		@param[in]      vec             減算するvector3
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         減算結果
		@exception      none
	**********************************************************************/
	rotation3 operator-(vector3& vec) {
		rotation3 result;
		result.x = x - vec.x; result.y = y - vec.y; result.z = z - vec.z;
		return result;
	}
	rotation3 operator-(const vector3& vec) {
		rotation3 result;
		result.x = x - vec.x; result.y = y - vec.y; result.z = z - vec.z;
		return result;
	}

	/*********************************************************************/
	/*! @brief 各座標ごとに加算を行い、結果を自身に反映する

		@param[in]      rotat             加算するrotation3
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         加算結果
		@exception      none
	**********************************************************************/
	rotation3& operator+=(rotation3& rotat) {
		x += rotat.x; y += rotat.y; z += rotat.z;
		return (*this);
	}
	rotation3& operator+=(const rotation3& rotat) {
		x += rotat.x; y += rotat.y; z += rotat.z;
		return (*this);
	}

	/*********************************************************************/
	/*! @brief 各座標ごとに減算を行い、結果を自身に反映する

		@param[in]      rotat             減算するrotation3
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         減算結果
		@exception      none
	**********************************************************************/
	rotation3& operator-=(rotation3& rotat) {
		x -= rotat.x; y -= rotat.y; z -= rotat.z;
		return (*this);
	}
	rotation3& operator-=(const rotation3& rotat) {
		x -= rotat.x; y -= rotat.y; z -= rotat.z;
		return (*this);
	}

	/*********************************************************************/
	/*! @brief 各座標ごとに加算を行い、結果を自身に反映する

		@param[in]      vec             加算するvector3
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         加算結果
		@exception      none
	**********************************************************************/
	rotation3& operator+=(vector3& vec) {
		x += vec.x; y += vec.y; z += vec.z;
		return (*this);
	}
	rotation3& operator+=(const vector3& vec) {
		x += vec.x; y += vec.y; z += vec.z;
		return (*this);
	}

	/*********************************************************************/
	/*! @brief 各座標ごとに減算を行い、結果を自身に反映する

		@param[in]      vec             減算するvector3
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         減算結果
		@exception      none
	**********************************************************************/
	rotation3& operator-=(vector3& vec) {
		x -= vec.x; y -= vec.y; z -= vec.z;
		return (*this);
	}
	rotation3& operator-=(const vector3& vec) {
		x -= vec.x; y -= vec.y; z -= vec.z;
		return (*this);
	}
};


#endif // SHARAKU_UTILTY_ROTATION_H_
