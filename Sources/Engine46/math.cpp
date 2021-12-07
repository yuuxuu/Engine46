/**
 * @file math.cpp
 * @brief
 * @author 木村優
 * @date 2021/04/10
 */

#include "math.h"

namespace Engine46 {

    // ラジアンから度数へ変換
    float DegreeToRadian(float degree) { 
        return degree * (PI / 180.0f); 
    }

    // 度数からラジアンへ変換
    float RadianToDegree(float radian) { 
        return radian * (180.0f / PI); 
    }

    // VECTOR2の長さを取得
    float Vec2Length(VECTOR2 vec) {
        return std::sqrtf(vec.x * vec.x + vec.y * vec.y);
    }

    // VECTOR3の長さを取得
    float Vec3Lenght(VECTOR3 vec) {
        return std::sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }

    // VECTOR3を正規化
    VECTOR3 Vec3Normalize(VECTOR3 vec) {
        float len = Vec3Lenght(vec);

        return VECTOR3(vec.x / len, vec.y /len, vec.z / len);
    }

    // VECTOR3同士の内積
    float Vec3Dot(VECTOR3 vec1, VECTOR3 vec2) {
        return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    }

    // VECTOR3同士の外積
    VECTOR3 Vec3Cross(VECTOR3 vec1, VECTOR3 vec2) {
        return VECTOR3(vec1.y * vec2.z - vec1.z * vec2.y, vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x);
    }

    // VECTOR3と行列を計算
    VECTOR3 Vec3TransformCoord(VECTOR3 vec, Matrix matrix) {
        VECTOR3 outVec;

        outVec.x = vec.x * matrix._11 + vec.y * matrix._21 + vec.z * matrix._31 + matrix._41;
        outVec.y = vec.x * matrix._12 + vec.y * matrix._22 + vec.z * matrix._32 + matrix._42;
        outVec.z = vec.x * matrix._13 + vec.y * matrix._23 + vec.z * matrix._33 + matrix._43;

        float w = vec.x * matrix._14 + vec.y * matrix._24 + vec.z * matrix._34 + matrix._44;
        if (w != 0.0f) {
            outVec.x /= w;
            outVec.y /= w;
            outVec.z /= w;
        }

        return outVec;
    }

    // 視線方向に向いた回転行列を取得
    Matrix MatrixLookAt(VECTOR3 pos, VECTOR3 look, VECTOR3 up) {
        VECTOR3 z = look - pos;
        z = Vec3Normalize(z);

        VECTOR3 x = Vec3Cross(up, z);
        x = Vec3Normalize(x);

        VECTOR3 y = Vec3Cross(z, x);
        y = Vec3Normalize(y);

        Matrix mat;
        mat._11 = x.x; mat._12 = x.y; mat._13 = x.z;
        mat._21 = y.x; mat._22 = y.y; mat._23 = y.z;
        mat._31 = z.x; mat._32 = z.y; mat._33 = z.z;

        return mat;
    }

    // ビューポート行列を取得
    Matrix GetViewPortMatrix(float screenWidth, float screenHeight) {
        
        float w = screenWidth * 0.5f;
        float h = screenHeight * 0.5f;
        
        Matrix mat;
        mat._11 = w;
        mat._22 = -h;

        mat._41 = w;
        mat._42 = h;

        return mat;
    }
}