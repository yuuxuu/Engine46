/**
 * @file math.cpp
 * @brief
 * @author 木村優
 * @date 2021/04/10
 */

#include "math.h"

namespace Engine46 {

	// VECTOR3の長さを取得
	float Vec3Lenght(VECTOR3 vec) {
		return std::sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}

	// VECTOR3を正規化
	VECTOR3 Vec3Normalize(VECTOR3 vec) {
		float len = Vec3Lenght(vec);

		if (len > 0.0f){
			len = 1.0f / len;
		}

		return VECTOR3(vec.x * len, vec.y * len, vec.z * len);
	}

	// VECTOR3同士の内積
	float Vec3Dot(VECTOR3 vec1, VECTOR3 vec2) {
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
	}

	// VECTOR3同士の外積
	VECTOR3 Vec3Cross(VECTOR3 vec1, VECTOR3 vec2) {
		return VECTOR3(vec1.y * vec2.z - vec1.z * vec2.y, vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x);
	}

	// 視線方向に向いた行列を取得
	Matrix MatrixLookAt(VECTOR3 pos, VECTOR3 look, VECTOR3 up){
		VECTOR3 z = look - pos;
		z = Vec3Normalize(z);

		VECTOR3 x = Vec3Cross(up, z);
		x = Vec3Normalize(x);

		VECTOR3 y = Vec3Cross(z, x);
		y = Vec3Normalize(y);

		Matrix matLookAt;
		matLookAt._11 = x.x; matLookAt._12 = x.y; matLookAt._13 = x.z;
		matLookAt._21 = y.x; matLookAt._22 = y.y; matLookAt._23 = y.z;
		matLookAt._31 = z.x; matLookAt._32 = z.y; matLookAt._33 = z.z;

		return matLookAt;
	}
}