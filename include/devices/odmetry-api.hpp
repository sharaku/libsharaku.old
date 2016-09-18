/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_DEVICE_ODMETRY_API_H_
#define SHARAKU_DEVICE_ODMETRY_API_H_

#include <stdint.h>
#include <sharaku/types.h>
#include <sharaku/position.hpp>
#include <sharaku/rotation.hpp>

NAMESPACE_SHARAKU_BEGIN

//-----------------------------------------------------------------------------
// 座標計測インタフェース
class odmetry_operations
{
 public:
	enum uint {
		USONIC_MM,
		USONIC_IN,
	};
	virtual const position3& get_position(void) {
		static position3 pos; pos(0.0f, 0.0f, 0.0f); return pos;
	}
	virtual int32_t set_position(const position3& pos) {return -ENOTSUP;}
	virtual const rotation3& get_rotation(void) {
		static rotation3 rot; rot(0.0f, 0.0f, 0.0f); return rot;
	}
	virtual int32_t set_rotation(const rotation3& rot) {return -ENOTSUP;}
	virtual const int32_t get_dist(void) {return -ENOTSUP;}
	virtual int32_t set_dist(int32_t dist) {return -ENOTSUP;}
	virtual int32_t reset(void) {return -ENOTSUP;}
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_DEVICE_ODMETRY_API_H_
