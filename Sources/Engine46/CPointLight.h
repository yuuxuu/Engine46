/**
 * @file CPointLight.h
 * @brief
 * @author 木村優
 * @date 2021/08/02
 */

#pragma once

#include "CLight.h"

namespace Engine46 {

    class CPointLight : public CLight {
    private:
        float m_radius;

    public:
        CPointLight(const std::string& lightName);
        ~CPointLight();

        void Update() override;

        void SetRadius(float radius) { m_radius = radius; }
        float GetRadius() const { return m_radius; }
    };

} // namespace


