/**
 * @file COBB.cpp
 * @brief
 * @author 木村優
 * @date 2021/08/10
 */

#include "COBB.h"
#include "CActor.h"
#include "CMesh.h"

namespace Engine46 {

    // コンストラクタ
    COBB::COBB()
    {}

    // デストラクタ
    COBB::~COBB()
    {}

    // 初期化
    void COBB::Initialize(MeshInfo& meshInfo, Transform& transform) {

        Matrix matRotate;
        matRotate.dx_m = DirectX::XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

        m_c = meshInfo.vertexCenterPos + transform.pos;

        m_u[0] = VECTOR3(matRotate._11, matRotate._12, matRotate._13);
        m_u[1] = VECTOR3(matRotate._21, matRotate._22, matRotate._23);
        m_u[2] = VECTOR3(matRotate._31, matRotate._32, matRotate._33);

        m_e = meshInfo.vertexE * transform.scale;
    }

    // 更新
    void COBB::Update(CActorBase* pActor) {

        if (!pActor) return;

        Transform transform = pActor->GetTransform();

        Matrix matRotate;
        matRotate.dx_m = DirectX::XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z);

        m_u[0] = VECTOR3(matRotate._11, matRotate._12, matRotate._13);
        m_u[1] = VECTOR3(matRotate._21, matRotate._22, matRotate._23);
        m_u[2] = VECTOR3(matRotate._31, matRotate._32, matRotate._33);

        CMeshBase* pMesh = pActor->GetMesh();
        if (!pMesh) return;

        MeshInfo meshInfo = pMesh->GetMeshInfo();

        m_c = meshInfo.vertexCenterPos + transform.pos;

        m_e = meshInfo.vertexE * transform.scale;
    }

    // レイがヒットしたか？
    bool COBB::IsRayHit(Ray& ray) {

        VECTOR3 m = (ray.orgRay + ray.dirRay) * 0.5f;
        VECTOR3 d = ray.dirRay - m;

        m = m - m_c;
        
        m = VECTOR3(Vec3Dot(m_u[0], m), Vec3Dot(m_u[1], m), Vec3Dot(m_u[2], m));
        d = VECTOR3(Vec3Dot(m_u[0], d), Vec3Dot(m_u[1], d), Vec3Dot(m_u[2], d));

        float adx = fabsf(d.x);
        if (fabsf(m.x) > m_e.x + adx) return false;
        float ady = fabsf(d.y);
        if (fabsf(m.y) > m_e.y + ady) return false;
        float adz = fabsf(d.z);
        if (fabsf(m.z) > m_e.z + adz) return false;

        const float epsilon = static_cast<float>(1.175494e-37);
        adx += epsilon;
        ady += epsilon;
        adz += epsilon;

        if (fabsf(m.y * d.z - m.z * d.y) > m_e.y * adz + m_e.z * ady) return false;
        if (fabsf(m.z * d.x - m.x * d.z) > m_e.x * adz + m_e.z * adx) return false;
        if (fabsf(m.x * d.y - m.y * d.x) > m_e.x * ady + m_e.y * adx) return false;

        return true;
    }

} // namespace
