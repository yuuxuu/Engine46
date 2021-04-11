/**
 * @file math.h
 * @brief
 * @author –Ø‘º—D
 * @date 2021/04/10
 */

#pragma once

#ifndef _MATH_H_
#define _MATH_H_

namespace Engine46 {
	struct Float2 {
		float x, y;

		Float2() {
			x = y = 0;
		}
		Float2(float f1, float f2) {
			x = f1; y = f2;
		}
		Float2 operator = (Float2 f) {
			x = f.x; y = f.y;
			return Float2(x, y);
		};
		Float2 operator + (Float2 f) {
			x += f.x; y += f.y;
			return Float2(x, y);
		};
		Float2 operator - (Float2 f) {
			x -= f.x; y -= f.y;
			return Float2(x, y);
		};
		Float2 operator * (Float2 f) {
			x *= f.x; y *= f.y;
			return Float2(x, y);
		};
		Float2 operator / (Float2 f) {
			x /= f.x; y /= f.y;
			return Float2(x, y);
		};
		Float2 operator += (Float2 f) {
			x += f.x; y += f.y;
			return Float2(x, y);
		};
		Float2 operator -= (Float2 f) {
			x -= f.x; y -= f.y;
			return Float2(x, y);
		};
		Float2 operator *= (Float2 f) {
			x *= f.x; y *= f.y;
			return Float2(x, y);
		};
		Float2 operator /= (Float2 f) {
			x /= f.x; y /= f.y;
			return Float2(x, y);
		};
	};

	struct Float3 {
		float x, y, z;

		Float3() {
			x = y = z = 0;
		}
		Float3(float f1, float f2, float f3) {
			x = f1; y = f2; z = f3;
		}
		Float3 operator = (Float3 f) {
			x = f.x; y = f.y; z = f.z;
			return Float3(x, y, z);
		};
		Float3 operator + (Float3 f) {
			x += f.x; y += f.y; z += f.z;
			return Float3(x, y, z);
		};
		Float3 operator - (Float3 f) {
			x -= f.x; y -= f.y; z -= f.z;
			return Float3(x, y, z);
		};
		Float3 operator * (Float3 f) {
			x *= f.x; y *= f.y; z *= f.z;
			return Float3(x, y, z);
		};
		Float3 operator / (Float3 f) {
			x /= f.x; y /= f.y; z /= f.z;
			return Float3(x, y, z);
		};
		Float3 operator += (Float3 f) {
			x += f.x; y += f.y; z += f.z;
			return Float3(x, y, z);
		};
		Float3 operator -= (Float3 f) {
			x -= f.x; y -= f.y; z -= f.z;
			return Float3(x, y, z);
		};
		Float3 operator *= (Float3 f) {
			x *= f.x; y *= f.y; z *= f.z;
			return Float3(x, y, z);
		};
		Float3 operator /= (Float3 f) {
			x /= f.x; y /= f.y; z /= f.z;
			return Float3(x, y, z);
		};
		void Float3Cross(Float3 f0, Float3 f1) {
			x = f0.y * f1.z - f0.z * f1.y;
			y = f0.z * f1.x - f0.x * f1.z;
			z = f0.x * f1.y - f0.y * f1.x;
		};
	};

	struct Float4 {
		float x, y, z, w;

		Float4() {
			x = y = z = w = 0;
		}
		Float4(float f1, float f2, float f3, float f4) {
			x = f1; y = f2; z = f3; w = f4;
		}
		Float4 operator = (Float4 f) {
			x = f.x; y = f.y; z = f.z; w = f.w;
			return Float4(x, y, z, w);
		}
		Float4 operator + (Float4 f) {
			x += f.x; y += f.y; z += f.z; w += f.w;
			return Float4(x, y, z, w);
		};
		Float4 operator - (Float4 f) {
			x -= f.x; y -= f.y; z -= f.z; w -= f.w;
			return Float4(x, y, z, w);
		};
		Float4 operator * (Float4 f) {
			x *= f.x; y *= f.y; z *= f.z; w *= f.w;
			return Float4(x, y, z, w);
		};
		Float4 operator / (Float4 f) {
			x /= f.x; y /= f.y; z /= f.z; w /= f.w;
			return Float4(x, y, z, w);
		};
		Float4 operator += (Float4 f) {
			x += f.x; y += f.y; z += f.z; w += f.w;
			return Float4(x, y, z, w);
		};
		Float4 operator -= (Float4 f) {
			x -= f.x; y -= f.y; z -= f.z; w -= f.w;
			return Float4(x, y, z, w);
		};
		Float4 operator *= (Float4 f) {
			x *= f.x; y *= f.y; z *= f.z; w *= f.w;
			return Float4(x, y, z, w);
		};
		Float4 operator /= (Float4 f) {
			x /= f.x; y /= f.y; z /= f.z; w /= f.w;
			return Float4(x, y, z, w);
		};
	};
} // namespace

#endif
