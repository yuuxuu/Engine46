/**
 * @file CMesh.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/06
 */

#include "CMesh.h"
#include "utility.h"

namespace Engine46 {

    // コンストラクタ
    CMeshBase::CMeshBase() :
        m_meshID(0),
        m_meshName("Mesh_" + std::to_string(m_meshID)),
        m_isInitialize(false)
    {
        m_meshName.resize(m_meshName.size());
    }

    // コンストラクタ
    CMeshBase::CMeshBase(const char* meshName) :
        m_meshID(0),
        m_meshName(meshName),
        m_isInitialize(false)
    {
        m_meshName.resize(m_meshName.size());
    }

    // デストラクタ
    CMeshBase::~CMeshBase()
    {
        VecClear(m_vecVertexInfo);

        VecClear(m_vecIndexes);
    }

    // スプライトメッシュ作成
    void CMeshBase::CreateSpriteMesh() {

        if (m_isInitialize) return;

        m_vecVertexInfo.reserve(4);

        vertexInfo info = {
            VECTOR3(-1.0f, 1.0f, 0.0f),
            VECTOR4(1.0f, 1.0f, 1.0f, 1.0f),
            VECTOR2(0.0f, 0.0f),
            VECTOR3(0.0f, 0.0f, 1.0f),
        };
        m_vecVertexInfo.emplace_back(info);

        info = {
            VECTOR3(1.0f, 1.0f, 0.0f),
            VECTOR4(1.0f, 1.0f, 1.0f, 1.0f),
            VECTOR2(1.0f, 0.0f),
            VECTOR3(0.0f, 0.0f, 1.0f),
        };
        m_vecVertexInfo.emplace_back(info);

        info = {
            VECTOR3(-1.0f, -1.0f, 0.0f),
            VECTOR4(1.0f, 1.0f, 1.0f, 1.0f),
            VECTOR2(0.0f, 1.0f),
            VECTOR3(0.0f, 0.0f, 1.0f),
        };
        m_vecVertexInfo.emplace_back(info);

        info = {
            VECTOR3(1.0f, -1.0f, 0.0f),
            VECTOR4(1.0f, 1.0f, 1.0f, 1.0f),
            VECTOR2(1.0f, 1.0f),
            VECTOR3(0.0f, 0.0f, 1.0f),
        };
        m_vecVertexInfo.emplace_back(info);

        CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE::TRIANGLESTRIP);

        m_vecIndexes.reserve(6);

        m_vecIndexes.emplace_back(0);
        m_vecIndexes.emplace_back(1);
        m_vecIndexes.emplace_back(3);
        m_vecIndexes.emplace_back(0);
        m_vecIndexes.emplace_back(3);
        m_vecIndexes.emplace_back(2);

        CreateIndexBuffer();
    }

} // namespace