/*
 * Copyright Abe Takafumi All Rights Reserved, 2004-2016
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_MM_VECTOR_H_
#define SHARAKU_MM_VECTOR_H_

struct vector3 {
 public:
	float x; ///< xベクトル
	float y; ///< yベクトル
	float z; ///< zベクトル

 public:
	/*********************************************************************/
	/*! @brief x, y, zを代入する

		@param[in]      vec_x           新規設定を行うXベクトル
		@param[in]      vec_y           新規設定を行うYベクトル
		@param[in]      vec_z           新規設定を行うZベクトル(省略時は0.0fとなる)
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         設定後の3次元ベクトル(移動量)構造体
		@exception      none
	**********************************************************************/
	vector3& operator()(float vec_x, float vec_y, float vec_z = 0.0f) {
		x = vec_x; y = vec_y; z = vec_z;
		return (*this);
	}

	/*********************************************************************/
	/*! @brief vector3構造体を代入する

		@param[in]      vec             代入する構造体
		@par            Refer
		- 参照するグローバル変数 none
		@par            Modify
		- 変更するグローバル変数 none
		@return         設定後の3次元ベクトル(移動量)構造体
		@exception      none
	**********************************************************************/
	vector3& operator=(vector3& vec) {
		x = vec.x; y = vec.y; z = vec.z;
		return (*this);
	}
	vector3& operator=(const vector3& vec) {
		x = vec.x; y = vec.y; z = vec.z;
		return (*this);
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
	vector3 operator+(vector3& vec) {
		vector3 result;
		result.x = x + vec.x; result.y = y + vec.y; result.z = z + vec.z;
		return result;
	}
	vector3 operator+(const vector3& vec) {
		vector3 result;
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
	vector3 operator-(vector3& vec) {
		vector3 result;
		result.x = x - vec.x; result.y = y - vec.y; result.z = z - vec.z;
		return result;
	}
	vector3 operator-(const vector3& vec) {
		vector3 result;
		result.x = x - vec.x; result.y = y - vec.y; result.z = z - vec.z;
		return result;
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
	vector3& operator+=(vector3& vec) {
		x += vec.x; y += vec.y; z += vec.z;
		return (*this);
	}
	vector3& operator+=(const vector3& vec) {
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
	vector3& operator-=(vector3& vec) {
		x -= vec.x; y -= vec.y; z -= vec.z;
		return (*this);
	}
	vector3& operator-=(const vector3& vec) {
		x -= vec.x; y -= vec.y; z -= vec.z;
		return (*this);
	}
};


#endif // SHARAKU_MM_VECTOR_H_
