/**
 * @file CConstantBuffer.h
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#pragma once

#ifndef _CCONSTANT_BUFFER_H_
#define _CCONSTANT_BUFFER_H_

#include "math.h"

namespace Engine46 {

    enum class CB_TYPE {
        WORLD,
        MATERIAL,
        CAMERA,
        DIRECTIONAL_LIGHT,
        POINT_LIGHT,
        SPOT_LIGHT,
    };

    struct worldCB {
        Matrix	matW;
    };

    struct materialCB {
        VECTOR4	diffuse;
        VECTOR4	specular;
        VECTOR4	ambient;
        VECTOR4	emissive;
    };

    struct CameraCB {
        Matrix	matVP;
        VECTOR3	cameraPos;
    };

    struct DirectionalLightCB {
        VECTOR3 pos;
        float	dummy;
        VECTOR4 diffuse;
        VECTOR4 specular;
    };

    constexpr UINT LIGHT_MAX = 1024 / 2;

    struct PointLightCB {
        struct PointLight {
            VECTOR3 pos;
            float	radius;
            VECTOR4 diffuse;
            VECTOR4 specular;
            VECTOR4 attenuation;
        };

        PointLightCB() :
            numPointLight(0)
        {}

        PointLight pointLights[LIGHT_MAX];
        int numPointLight;
        int dummy1;
        int dummy2;
        int dummy3;
    };

    struct SpotLightCB {
        struct SpotLight {
            VECTOR3 pos;
            float	angle;
            VECTOR4 diffuse;
            VECTOR4 specular;
            VECTOR4 attenuation;
        };

        SpotLightCB() :
            numSpotLight(0)
        {}

        SpotLight spotLights[LIGHT_MAX];
        int numSpotLight;
        int dummy1;
        int dummy2;
        int dummy3;
    };

    constexpr UINT OFFSET_MAX = 15;

    struct PostEffectCB {
        VECTOR4 blurOffset[OFFSET_MAX];

        UINT texWidth;
        UINT texHeight;
    };

    class CConstantBufferBase {
    public:
        CConstantBufferBase();
        virtual ~CConstantBufferBase();

        virtual void CreateConstantBuffer(UINT byteWidth) {};
        virtual void CreateConstantBufferView() {};
        virtual void Update(void* srcData) {};
        virtual void Set(UINT slot) {};
        virtual void SetCompute(UINT slot) {};
    };

} // namespace

#endif
