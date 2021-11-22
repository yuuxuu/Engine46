/**
 * @file CMesh.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/06
 */

#include "CMesh.h"
#include "CMaterial.h"
#include "utility.h"

#include "CGameSystem.h"
#include "CMaterialManager.h"

namespace Engine46 {

    // コンストラクタ
    CMeshBase::CMeshBase() :
        m_meshID(0),
        m_meshName("Mesh_" + std::to_string(m_meshID))
    {
        m_meshName.resize(m_meshName.size());
    }

    // コンストラクタ
    CMeshBase::CMeshBase(const char* meshName) :
        m_meshID(0),
        m_meshName(meshName)
    {
        m_meshName.resize(m_meshName.size());
    }

    // デストラクタ
    CMeshBase::~CMeshBase()
    {
        VecClear(m_vecVertexInfo);

        VecClear(m_vecIndex);
    }

    void CMeshBase::Set() {
        if (pMaterial) {
            pMaterial->Update();
            pMaterial->Set((UINT)CB_TYPE::MATERIAL);
        }
        else {
            for (const auto pMaterial : m_pVecMaterial) {
                pMaterial->Update();
                pMaterial->Set((UINT)CB_TYPE::MATERIAL);
            }
        }
    }

    // マテリアルを設定
    void CMeshBase::SetMaterial(const std::string& materialName) {
        CMaterialManager* materialManager = CGameSystem::GetGameSystem().GetMaterialManager();
        CMaterialBase* pMaterial = materialManager->CreateMaterial(materialName.c_str());
        if (pMaterial) {
            this->pMaterial = pMaterial;
        }
    }

    // メッシュ情報を設定
    void CMeshBase::SetMeshInfo(std::vector<VertexInfo>& vecVertex) {

        if (vecVertex.empty()) return;

        for (const auto& vertexInfo : vecVertex) {
            if (m_meshInfo.maxVertexPos.x < vertexInfo.vertex.x) m_meshInfo.maxVertexPos.x = vertexInfo.vertex.x;
            if (m_meshInfo.maxVertexPos.y < vertexInfo.vertex.y) m_meshInfo.maxVertexPos.y = vertexInfo.vertex.y;
            if (m_meshInfo.maxVertexPos.z < vertexInfo.vertex.z) m_meshInfo.maxVertexPos.z = vertexInfo.vertex.z;

            if (m_meshInfo.minVertexPos.x > vertexInfo.vertex.x) m_meshInfo.minVertexPos.x = vertexInfo.vertex.x;
            if (m_meshInfo.minVertexPos.y > vertexInfo.vertex.y) m_meshInfo.minVertexPos.y = vertexInfo.vertex.y;
            if (m_meshInfo.minVertexPos.z > vertexInfo.vertex.z) m_meshInfo.minVertexPos.z = vertexInfo.vertex.z;
        }

        m_meshInfo.vertexCenterPos = (m_meshInfo.minVertexPos + m_meshInfo.maxVertexPos) * 0.5f;

        m_meshInfo.vertexE.x = fabsf(m_meshInfo.maxVertexPos.x - m_meshInfo.minVertexPos.x) * 0.5f;
        m_meshInfo.vertexE.y = fabsf(m_meshInfo.maxVertexPos.y - m_meshInfo.minVertexPos.y) * 0.5f;
        m_meshInfo.vertexE.z = fabsf(m_meshInfo.maxVertexPos.z - m_meshInfo.minVertexPos.z) * 0.5f;
    }

    // スプライトメッシュ作成
    void CMeshBase::CreateSpriteMesh() {

        const float r = 1.0f;

        m_vecVertexInfo.reserve(4);

        m_vecVertexInfo = {
            // vertex               // color                         // uv                // normal
            { VECTOR3(-r, r, 0.0f), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(0.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f), },
            { VECTOR3( r, r, 0.0f), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f), },
            { VECTOR3(-r,-r, 0.0f), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(0.0f, 1.0f), VECTOR3(0.0f, 0.0f, 1.0f), },
            { VECTOR3( r,-r, 0.0f), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 1.0f), VECTOR3(0.0f, 0.0f, 1.0f), },
        };

        CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE::TRIANGLESTRIP);

        m_vecIndex.reserve(6);

        m_vecIndex = {
            0, 1, 2,
            1, 2, 3,
        };

        CreateIndexBuffer();

        SetMeshInfo(m_vecVertexInfo);
    }

    // ボックスメッシュ作成
    void CMeshBase::CreateBoxMesh() {

        const float r = 1.0f;

        m_vecVertexInfo.reserve(24);

        m_vecVertexInfo = {
            // vertex            // color                         // uv                // normal
            // +Z
            { VECTOR3( r, r, r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(0.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f), },
            { VECTOR3(-r, r, r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f), },
            { VECTOR3( r,-r, r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(0.0f, 1.0f), VECTOR3(0.0f, 0.0f, 1.0f), },
            { VECTOR3(-r,-r, r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 1.0f), VECTOR3(0.0f, 0.0f, 1.0f), },
            // -Z
            { VECTOR3(-r, r,-r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(0.0f, 0.0f), VECTOR3(0.0f, 0.0f, -1.0f), },
            { VECTOR3( r, r,-r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 0.0f), VECTOR3(0.0f, 0.0f, -1.0f), },
            { VECTOR3(-r,-r,-r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(0.0f, 1.0f), VECTOR3(0.0f, 0.0f, -1.0f), },
            { VECTOR3( r,-r,-r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 1.0f), VECTOR3(0.0f, 0.0f, -1.0f), },
            // +X
            { VECTOR3( r, r,-r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(0.0f, 0.0f), VECTOR3(1.0f, 0.0f, 0.0f), },
            { VECTOR3( r, r, r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 0.0f), VECTOR3(1.0f, 0.0f, 0.0f), },
            { VECTOR3( r,-r,-r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(0.0f, 1.0f), VECTOR3(1.0f, 0.0f, 0.0f), },
            { VECTOR3( r,-r, r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 1.0f), VECTOR3(1.0f, 0.0f, 0.0f), },
            // -X
            { VECTOR3(-r, r, r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(0.0f, 0.0f), VECTOR3(-1.0f, 0.0f, 0.0f), },
            { VECTOR3(-r, r,-r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 0.0f), VECTOR3(-1.0f, 0.0f, 0.0f), },
            { VECTOR3(-r,-r, r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(0.0f, 1.0f), VECTOR3(-1.0f, 0.0f, 0.0f), },
            { VECTOR3(-r,-r,-r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 1.0f), VECTOR3(-1.0f, 0.0f, 0.0f), },
            // +Y
            { VECTOR3(-r, r, r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(0.0f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f), },
            { VECTOR3( r, r, r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f), },
            { VECTOR3(-r, r,-r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(0.0f, 1.0f), VECTOR3(0.0f, 1.0f, 0.0f), },
            { VECTOR3( r, r,-r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 1.0f), VECTOR3(0.0f, 1.0f, 0.0f), },
            // -Y
            { VECTOR3(-r,-r, r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(0.0f, 0.0f), VECTOR3(0.0f, -1.0f, 0.0f), },
            { VECTOR3( r,-r, r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 0.0f), VECTOR3(0.0f, -1.0f, 0.0f), },
            { VECTOR3(-r,-r,-r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(0.0f, 1.0f), VECTOR3(0.0f, -1.0f, 0.0f), },
            { VECTOR3( r,-r,-r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 1.0f), VECTOR3(0.0f, -1.0f, 0.0f), },
        };

        CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE::TRIANGLELIST);

        m_vecIndex.reserve(36);

        m_vecIndex = {
            0, 1, 2, 
            3, 2, 1,

            4, 5, 6, 
            7, 6, 5,

            8, 9, 10, 
            11, 10, 9,

            12, 13, 14, 
            15, 14, 13,

            16, 17, 18, 
            19, 18, 17,

            20, 21, 22, 
            23, 22, 21,
        };

        CreateIndexBuffer();

        SetMeshInfo(m_vecVertexInfo);
    }

} // namespace