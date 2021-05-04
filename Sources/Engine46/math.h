/**
 * @file math.h
 * @brief
 * @author 木村優
 * @date 2021/04/10
 */

#pragma once

#ifndef _MATH_H_
#define _MATH_H_

namespace Engine46 {

	struct VECTOR2 {
		float x, y;

		VECTOR2() {
			x = y = 0;
		}
		VECTOR2(float f1, float f2) {
			x = f1; y = f2;
		}
		VECTOR2 operator = (VECTOR2 f) {
			x = f.x; y = f.y;
			return VECTOR2(x, y);
		};
		VECTOR2 operator + (VECTOR2 f) {
			x += f.x; y += f.y;
			return VECTOR2(x, y);
		};
		VECTOR2 operator - (VECTOR2 f) {
			x -= f.x; y -= f.y;
			return VECTOR2(x, y);
		};
		VECTOR2 operator * (VECTOR2 f) {
			x *= f.x; y *= f.y;
			return VECTOR2(x, y);
		};
		VECTOR2 operator / (VECTOR2 f) {
			x /= f.x; y /= f.y;
			return VECTOR2(x, y);
		};
		VECTOR2 operator += (VECTOR2 f) {
			x += f.x; y += f.y;
			return VECTOR2(x, y);
		};
		VECTOR2 operator -= (VECTOR2 f) {
			x -= f.x; y -= f.y;
			return VECTOR2(x, y);
		};
		VECTOR2 operator *= (VECTOR2 f) {
			x *= f.x; y *= f.y;
			return VECTOR2(x, y);
		};
		VECTOR2 operator /= (VECTOR2 f) {
			x /= f.x; y /= f.y;
			return VECTOR2(x, y);
		};
	};

	struct VECTOR3 {
		float x, y, z;

		VECTOR3() {
			x = y = z = 0;
		}
		VECTOR3(float f1, float f2, float f3) {
			x = f1; y = f2; z = f3;
		}
		VECTOR3 operator = (VECTOR3 f) {
			x = f.x; y = f.y; z = f.z;
			return VECTOR3(x, y, z);
		};
		VECTOR3 operator + (VECTOR3 f) {
			x += f.x; y += f.y; z += f.z;
			return VECTOR3(x, y, z);
		};
		VECTOR3 operator - (VECTOR3 f) {
			x -= f.x; y -= f.y; z -= f.z;
			return VECTOR3(x, y, z);
		};
		VECTOR3 operator * (VECTOR3 f) {
			x *= f.x; y *= f.y; z *= f.z;
			return VECTOR3(x, y, z);
		};
		VECTOR3 operator / (VECTOR3 f) {
			x /= f.x; y /= f.y; z /= f.z;
			return VECTOR3(x, y, z);
		};
		VECTOR3 operator += (VECTOR3 f) {
			x += f.x; y += f.y; z += f.z;
			return VECTOR3(x, y, z);
		};
		VECTOR3 operator -= (VECTOR3 f) {
			x -= f.x; y -= f.y; z -= f.z;
			return VECTOR3(x, y, z);
		};
		VECTOR3 operator *= (VECTOR3 f) {
			x *= f.x; y *= f.y; z *= f.z;
			return VECTOR3(x, y, z);
		};
		VECTOR3 operator /= (VECTOR3 f) {
			x /= f.x; y /= f.y; z /= f.z;
			return VECTOR3(x, y, z);
		};
		void Float3Cross(VECTOR3 f0, VECTOR3 f1) {
			x = f0.y * f1.z - f0.z * f1.y;
			y = f0.z * f1.x - f0.x * f1.z;
			z = f0.x * f1.y - f0.y * f1.x;
		};
	};

	struct VECTOR4 {
		float x, y, z, w;

		VECTOR4() {
			x = y = z = w = 0;
		}
		VECTOR4(float f1, float f2, float f3, float f4) {
			x = f1; y = f2; z = f3; w = f4;
		}
		VECTOR4 operator = (VECTOR4 f) {
			x = f.x; y = f.y; z = f.z; w = f.w;
			return VECTOR4(x, y, z, w);
		}
		VECTOR4 operator + (VECTOR4 f) {
			x += f.x; y += f.y; z += f.z; w += f.w;
			return VECTOR4(x, y, z, w);
		};
		VECTOR4 operator - (VECTOR4 f) {
			x -= f.x; y -= f.y; z -= f.z; w -= f.w;
			return VECTOR4(x, y, z, w);
		};
		VECTOR4 operator * (VECTOR4 f) {
			x *= f.x; y *= f.y; z *= f.z; w *= f.w;
			return VECTOR4(x, y, z, w);
		};
		VECTOR4 operator / (VECTOR4 f) {
			x /= f.x; y /= f.y; z /= f.z; w /= f.w;
			return VECTOR4(x, y, z, w);
		};
		VECTOR4 operator += (VECTOR4 f) {
			x += f.x; y += f.y; z += f.z; w += f.w;
			return VECTOR4(x, y, z, w);
		};
		VECTOR4 operator -= (VECTOR4 f) {
			x -= f.x; y -= f.y; z -= f.z; w -= f.w;
			return VECTOR4(x, y, z, w);
		};
		VECTOR4 operator *= (VECTOR4 f) {
			x *= f.x; y *= f.y; z *= f.z; w *= f.w;
			return VECTOR4(x, y, z, w);
		};
		VECTOR4 operator /= (VECTOR4 f) {
			x /= f.x; y /= f.y; z /= f.z; w /= f.w;
			return VECTOR4(x, y, z, w);
		};
	};

	struct Transform {
		VECTOR3 pos;
		VECTOR3 rotation;
		VECTOR3 scale;

		Transform() {}

		Transform(VECTOR3 pos, VECTOR3 rotation, VECTOR3 scale) :
			pos(pos),
			rotation(rotation),
			scale(scale)
		{}
	};

	struct RECT {
		int w, h;

		RECT() {
			w = h = 0;
		}
		RECT(int f1, int f2) {
			w = f1; h = f2;
		}
		RECT operator = (RECT f) {
			w = f.w; h = f.h;
			return RECT(w, h);
		};
		RECT operator + (RECT f) {
			w += f.w; h += f.h;
			return RECT(w, h);
		};
		RECT operator - (RECT f) {
			w -= f.w; h -= f.h;
			return RECT(w, h);
		};
		RECT operator * (RECT f) {
			w *= f.w; h *= f.h;
			return RECT(w, h);
		};
		RECT operator / (RECT f) {
			w /= f.w; h /= f.h;
			return RECT(w, h);
		};
		RECT operator += (RECT f) {
			w += f.w; h += f.h;
			return RECT(w, h);
		};
		RECT operator -= (RECT f) {
			w -= f.w; h -= f.h;
			return RECT(w, h);
		};
		RECT operator *= (RECT f) {
			w *= f.w; h *= f.h;
			return RECT(w, h);
		};
		RECT operator /= (RECT f) {
			w /= f.w; h /= f.h;
			return RECT(w, h);
		};
	};

} // namespace

#endif
