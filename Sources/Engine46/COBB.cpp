/**
 * @file COBB.cpp
 * @brief
 * @author 木村優
 * @date 2021/08/10
 */

#include "COBB.h"
#include "CActor.h"
#include "CMesh.h"
#include "CModelMesh.h"
#include "CConstantBuffer.h"

#include "CGameSystem.h"
#include "CMeshManager.h"
#include "CShaderManager.h"
#include "CShaderPackage.h"

namespace Engine46 {

    // コンストラクタ
    COBB::COBB()
    {}

    // デストラクタ
    COBB::~COBB()
    {}

    // 更新
    void COBB::Update(CActorBase* pActor) {

        if (!pActor) return;

        Transform transform = pActor->GetTransform();

        Matrix matRotate;
        matRotate.dx_m = DirectX::XMMatrixRotationRollPitchYaw(transform.rotate.x, transform.rotate.y, transform.rotate.z);

        m_u[0] = VECTOR3(matRotate._11, matRotate._12, matRotate._13);
        m_u[1] = VECTOR3(matRotate._21, matRotate._22, matRotate._23);
        m_u[2] = VECTOR3(matRotate._31, matRotate._32, matRotate._33);

        CMeshBase* pMesh = pActor->GetMesh();
        if (pMesh) {
            MeshInfo meshInfo = pMesh->GetMeshInfo();

            m_c = meshInfo.vertexCenterPos + transform.pos;

            m_e = meshInfo.vertexE * transform.scale;
        }
        else {
            CModelMesh* pModelMesh = pActor->GetModelMesh();
            if (pModelMesh->GetVecMesh().empty()) return;

            MeshInfo meshInfo;
            for (const auto pMesh : pModelMesh->GetVecMesh()) {
                MeshInfo modelMeshInfo = pMesh->GetMeshInfo();

                if (meshInfo.maxVertexPos.x < modelMeshInfo.maxVertexPos.x) meshInfo.maxVertexPos.x = modelMeshInfo.maxVertexPos.x;
                if (meshInfo.maxVertexPos.y < modelMeshInfo.maxVertexPos.y) meshInfo.maxVertexPos.y = modelMeshInfo.maxVertexPos.y;
                if (meshInfo.maxVertexPos.z < modelMeshInfo.maxVertexPos.z) meshInfo.maxVertexPos.z = modelMeshInfo.maxVertexPos.z;

                if (meshInfo.minVertexPos.x > modelMeshInfo.minVertexPos.x) meshInfo.minVertexPos.x = modelMeshInfo.minVertexPos.x;
                if (meshInfo.minVertexPos.y > modelMeshInfo.minVertexPos.y) meshInfo.minVertexPos.y = modelMeshInfo.minVertexPos.y;
                if (meshInfo.minVertexPos.z > modelMeshInfo.minVertexPos.z) meshInfo.minVertexPos.z = modelMeshInfo.minVertexPos.z;
            }

            meshInfo.vertexCenterPos = (meshInfo.minVertexPos + meshInfo.maxVertexPos) * 0.5f;

            meshInfo.vertexE.x = fabsf(meshInfo.maxVertexPos.x - meshInfo.minVertexPos.x) * 0.5f;
            meshInfo.vertexE.y = fabsf(meshInfo.maxVertexPos.y - meshInfo.minVertexPos.y) * 0.5f;
            meshInfo.vertexE.z = fabsf(meshInfo.maxVertexPos.z - meshInfo.minVertexPos.z) * 0.5f;

            m_c = meshInfo.vertexCenterPos + transform.pos;

            m_e = meshInfo.vertexE * transform.scale;
        }
    }

    // 描画
    void COBB::Draw(CActorBase* pActor) {
        if (pMesh) {
            pMesh->Draw();
        }
    }

    // OBBメッシュを作成
    void COBB::CreateOBBMesh(const std::string& meshName) {
        
        if (!pMesh) {
            CMeshManager* manager = CGameSystem::GetGameSystem().GetMeshManager();
            if (!manager) return;

            pMesh = manager->CreateMesh(meshName);
            if (!pMesh) return;
        }

        VECTOR4 color(1.0f, 0.0f, 0.0f, 1.0f);

        // OBBの描画調整中
        std::vector<VertexInfo> vecVertex = {
            // vertex
            { VECTOR3(m_c.x + m_e.x, m_c.y + m_e.y, m_c.z - m_e.z), color},
            { VECTOR3(m_c.x + m_e.x, m_c.y - m_e.y, m_c.z - m_e.z), color},

            { VECTOR3(m_c.x - m_e.x, m_c.y + m_e.y, m_c.z - m_e.z), color},
            { VECTOR3(m_c.x - m_e.x, m_c.y - m_e.y, m_c.z - m_e.z), color},

            { VECTOR3(m_c.x - m_e.x, m_c.y + m_e.y, m_c.z + m_e.z), color},
            { VECTOR3(m_c.x - m_e.x, m_c.y - m_e.y, m_c.z + m_e.z), color},

            { VECTOR3(m_c.x + m_e.x, m_c.y + m_e.y, m_c.z + m_e.z), color},
            { VECTOR3(m_c.x + m_e.x, m_c.y - m_e.y, m_c.z + m_e.z), color},


            { VECTOR3(m_c.x + m_e.x, m_c.y + m_e.y, m_c.z - m_e.z), color},
            { VECTOR3(m_c.x - m_e.x, m_c.y + m_e.y, m_c.z - m_e.z), color},

            { VECTOR3(m_c.x + m_e.x, m_c.y + m_e.y, m_c.z + m_e.z), color},
            { VECTOR3(m_c.x - m_e.x, m_c.y + m_e.y, m_c.z + m_e.z), color},

            { VECTOR3(m_c.x + m_e.x, m_c.y - m_e.y, m_c.z + m_e.z), color},
            { VECTOR3(m_c.x - m_e.x, m_c.y - m_e.y, m_c.z + m_e.z), color},

            { VECTOR3(m_c.x + m_e.x, m_c.y - m_e.y, m_c.z - m_e.z), color},
            { VECTOR3(m_c.x - m_e.x, m_c.y - m_e.y, m_c.z - m_e.z), color},


            { VECTOR3(m_c.x + m_e.x, m_c.y + m_e.y, m_c.z + m_e.z), color},
            { VECTOR3(m_c.x + m_e.x, m_c.y + m_e.y, m_c.z - m_e.z), color},

            { VECTOR3(m_c.x + m_e.x, m_c.y - m_e.y, m_c.z + m_e.z), color},
            { VECTOR3(m_c.x + m_e.x, m_c.y - m_e.y, m_c.z - m_e.z), color},

            { VECTOR3(m_c.x - m_e.x, m_c.y + m_e.y, m_c.z + m_e.z), color},
            { VECTOR3(m_c.x - m_e.x, m_c.y + m_e.y, m_c.z - m_e.z), color},

            { VECTOR3(m_c.x - m_e.x, m_c.y - m_e.y, m_c.z + m_e.z), color},
            { VECTOR3(m_c.x - m_e.x, m_c.y - m_e.y, m_c.z - m_e.z), color},
        };

        pMesh->CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE::LINELIST, vecVertex);
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
