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
            return VECTOR2(x + f.x, y + f.y);
        };
        VECTOR2 operator - (VECTOR2 f) {
            return VECTOR2(x - f.x, y - f.y);
        };
        VECTOR2 operator * (VECTOR2 f) {
            return VECTOR2(x * f.x, y * f.y);
        };
        VECTOR2 operator * (float f) {
            return VECTOR2(x * f, y * f);
        };
        VECTOR2 operator / (VECTOR2 f) {
            return VECTOR2(x / f.x, y / f.y);
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
            return VECTOR3(x + f.x, y + f.y, z + f.z);
        };
        VECTOR3 operator + (float f) {
            x += f; y += f; z += f;
            return VECTOR3(x, y, z);
        };
        VECTOR3 operator - (VECTOR3 f) {
            return VECTOR3(x - f.x, y - f.y, z - f.z);
        };
        VECTOR3 operator * (VECTOR3 f) {
            return VECTOR3(x * f.x, y * f.y, z * f.z);
        };
        VECTOR3 operator * (float f) {
            return VECTOR3(x * f, y * f, z * f);
        };
        VECTOR3 operator / (VECTOR3 f) {
            return VECTOR3(x / f.x, y / f.y, z / f.z);
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
            return VECTOR4(x + f.x, y + f.y, z + f.z, w + f.w);
        };
        VECTOR4 operator - (VECTOR4 f) {
            return VECTOR4(x - f.x, y - f.y, z - f.z, w - f.w);
        };
        VECTOR4 operator * (VECTOR4 f) {
            return VECTOR4(x * f.x, y * f.y, z * f.z, w * f.w);
        };
        VECTOR4 operator / (VECTOR4 f) {
            return VECTOR4(x / f.x, y / f.y, z / f.z, w / f.w);
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

        Transform() :
            pos(),
            rotation(),
            scale(1.0f, 1.0f, 1.0f)
        {}

        Transform(VECTOR3 pos, VECTOR3 rotation, VECTOR3 scale) :
            pos(pos),
            rotation(rotation),
            scale(scale)
        {}
    };

    struct Matrix {
        union {
            struct {
                float _11, _12, _13, _14;
                float _21, _22, _23, _24;
                float _31, _32, _33, _34;
                float _41, _42, _43, _44;
            };
            float m[4][4];

            DirectX::XMMATRIX dx_m;
        };

        Matrix() {
            m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
            m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
            m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
            m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
        }
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

    float Vec3Lenght(VECTOR3 vec);

    VECTOR3 Vec3Normalize(VECTOR3 vec);

    float Vec3Dot(VECTOR3 vec1, VECTOR3 vec2);

    VECTOR3 Vec3Cross(VECTOR3 vec1, VECTOR3 vec2);

    Matrix MatrixLookAt(VECTOR3 pos, VECTOR3 look, VECTOR3 up);

} // namespace

#endif
