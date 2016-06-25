/*
 * Copyright Abe Takafumi All Rights Reserved, 2004-2016
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_MM_POSITION_H_
#define SHARAKU_MM_POSITION_H_

#include <sharaku/vector.hpp>

struct position3 {
 public:
	float x; ///< x座標
	float y; ///< y座標
	float z; ///< z座標

 public:
	/*********************************************************************/
	/*!@brief x, y, zを代入する
		@param[in]      pos_x           新規設定を行うX座標
		@param[in]      pos_y           新規設定を行うY座標
		@param[in]      pos_z           新規設定を行うZ座標(省略時は0.0fとなる)
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         設定後の3次元座標構造体
		@exception      none
	**********************************************************************/
	position3& operator()(float pos_x, float pos_y, float pos_z = 0.0f) {
		x = pos_x; y = pos_y; z = pos_z;
		return (*this);
	}

	/*********************************************************************/
	/*! @brief position3構造体を代入する

		@param[in]      pos             代入する構造体
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         設定後の3次元座標構造体
		@exception      none
	**********************************************************************/
	position3& operator=(position3& pos) {
		x = pos.x; y = pos.y; z = pos.z;
		return (*this);
	}
	position3& operator=(const position3& pos) {
		x = pos.x; y = pos.y; z = pos.z;
		return (*this);
	}

	/*********************************************************************/
	/*! @brief 各座標ごとに加算を行う

		@param[in]      pos             加算するposition3
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         加算結果
		@exception      none
	**********************************************************************/
	position3 operator+(position3& pos) {
		position3 result;
		result.x = x + pos.x; result.y = y + pos.y; result.z = z + pos.z;
		return result;
	}
	position3 operator+(const position3& pos) {
		position3 result;
		result.x = x + pos.x; result.y = y + pos.y; result.z = z + pos.z;
		return result;
	}

	/*********************************************************************/
	/*! @brief 各座標ごとに減算を行う

		@param[in]      pos             減算するposition3
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         減算結果
		@exception      none
	**********************************************************************/
	position3 operator-(position3& pos) {
		position3 result;
		result.x = x - pos.x; result.y = y - pos.y; result.z = z - pos.z;
		return result;
	}
	position3 operator-(const position3& pos) {
		position3 result;
		result.x = x - pos.x; result.y = y - pos.y; result.z = z - pos.z;
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
	position3 operator+(vector3& vec) {
		position3 result;
		result.x = x + vec.x; result.y = y + vec.y; result.z = z + vec.z;
		return result;
	}
	position3 operator+(const vector3& vec) {
		position3 result;
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
	position3 operator-(vector3& vec) {
		position3 result;
		result.x = x - vec.x; result.y = y - vec.y; result.z = z - vec.z;
		return result;
	}
	position3 operator-(const vector3& vec) {
		position3 result;
		result.x = x - vec.x; result.y = y - vec.y; result.z = z - vec.z;
		return result;
	}

	/*********************************************************************/
	/*! @brief 各座標ごとに加算を行い、結果を自身に反映する

		@param[in]      pos             加算するposition3
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         加算結果
		@exception      none
	**********************************************************************/
	position3& operator+=(position3& pos) {
		x += pos.x; y += pos.y; z += pos.z;
		return (*this);
	}
	position3& operator+=(const position3& pos) {
		x += pos.x; y += pos.y; z += pos.z;
		return (*this);
	}

	/*********************************************************************/
	/*! @brief 各座標ごとに減算を行い、結果を自身に反映する

		@param[in]      pos             減算するposition3
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         減算結果
		@exception      none
	**********************************************************************/
	position3& operator-=(position3& pos) {
		x -= pos.x; y -= pos.y; z -= pos.z;
		return (*this);
	}
	position3& operator-=(const position3& pos) {
		x -= pos.x; y -= pos.y; z -= pos.z;
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
	position3& operator+=(vector3& vec) {
		x += vec.x; y += vec.y; z += vec.z;
		return (*this);
	}
	position3& operator+=(const vector3& vec) {
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
	position3& operator-=(vector3& vec) {
		x -= vec.x; y -= vec.y; z -= vec.z;
		return (*this);
	}
	position3& operator-=(const vector3& vec) {
		x -= vec.x; y -= vec.y; z -= vec.z;
		return (*this);
	}
};


#endif // SHARAKU_MM_POSITION_H_
