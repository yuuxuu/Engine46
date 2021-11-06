/**
 * @file COBB.h
 * @brief
 * @author 木村優
 * @date 2021/08/10
 */

#pragma once

#include "math.h"
#include "CMesh.h"

namespace Engine46 {

    // 前方宣言
    class CActorBase;

    class COBB {
    private:
        VECTOR3 m_c;
        VECTOR3 m_u[3];
        VECTOR3 m_e;

    public:
        COBB();
        ~COBB();

        void Initialize(MeshInfo& meshInfo, Transform& transform);
        void Update(CActorBase* pActor);

        bool IsRayHit(Ray& ray);
    };

} // namespace
