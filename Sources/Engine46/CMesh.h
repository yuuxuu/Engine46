/**
 * @file CMesh.h
 * @brief
 * @author 木村優
 * @date 2021/05/06
 */

#pragma once

#ifndef _CMESH_H_
#define _CMESH_H_

#include "math.h"

namespace Engine46 {

    // 前方宣言
    class CMaterialBase;

    struct VertexInfo {
        VECTOR3	vertex;
        VECTOR4	color;
        VECTOR2	uv;
        VECTOR3	normal;
        VECTOR3	tangent;
        VECTOR3	binormal;
    };

    struct MeshInfo {
        VECTOR3 vertexE;
        VECTOR3 vertexCenterPos;
        VECTOR3 maxVertexPos;
        VECTOR3 minVertexPos;

        MeshInfo() :
            vertexE(0.0f, 0.0f, 0.0f),
            vertexCenterPos(0.0f, 0.0f, 0.0f),
            maxVertexPos(0.0f, 0.0f, 0.0f),
            minVertexPos(10000.0f, 10000.0f, 10000.0f)
        {}
    };

    enum class PRIMITIVE_TOPOLOGY_TYPE {
        POINTLIST,
        LINELIST,
        LINESTRIP,
        TRIANGLELIST,
        TRIANGLESTRIP,
    };

    class CMeshBase {
    protected:
        CMaterialBase*              pMaterial;

        std::vector<CMaterialBase*> m_pVecMaterial;

        std::vector<VertexInfo>	    m_vecVertexInfo;

        std::vector<DWORD>          m_vecIndex;

        MeshInfo                    m_meshInfo;

        int                         m_meshID;

        std::string                 m_meshName;

        UINT                        m_primitiveTopologyType;

        bool                        m_visible;

    public:
        CMeshBase();
        CMeshBase(const std::string& meshName);
        virtual ~CMeshBase();

        virtual void CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE type) {};
        virtual void CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE type, const std::vector<VertexInfo>& vecVertexInfo) {};
        virtual void CreateIndexBuffer() {};
        virtual void CreateIndexBuffer(const std::vector<DWORD>& vecIndex) {};
        virtual void Draw() {};

        void Set();

        CMaterialBase* GetMaterial() const { return pMaterial; }
        void SetMaterial(const std::string& materialName);
        void SetMaterial(CMaterialBase* pMaterial) { this->pMaterial = pMaterial; };

        std::vector<CMaterialBase*> GetVecMaterial() const { return m_pVecMaterial; }
        void AddMaterial(CMaterialBase* pMaterial) { m_pVecMaterial.emplace_back(pMaterial); }

        void SetMeshInfo(std::vector<VertexInfo>& vecVertex);

        void CreateSpriteMesh();
        void CreateBoxMesh();

        const MeshInfo& GetMeshInfo() const { return m_meshInfo; }

        std::string GetMeshName() const { return m_meshName; }

        bool GetVisible() const { return m_visible; }
        void SetVisible(bool visible) { m_visible = visible; }
    };

} // namespace

#endif
