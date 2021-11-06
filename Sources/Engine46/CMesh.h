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
        std::vector<VertexInfo>	m_vecVertexInfo;

        std::vector<DWORD>      m_vecIndexes;

        MeshInfo                m_meshInfo;

        int                     m_meshID;

        std::string             m_meshName;

        bool                    m_isInitialize;

        UINT                    m_primitiveTopologyType;

    public:
        CMeshBase();
        explicit CMeshBase(const char* meshName);
        virtual ~CMeshBase();

        virtual void CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE type) {};
        virtual void CreateIndexBuffer() {};
        virtual void Draw() {};

        void SetMeshInfo();

        void CreateSpriteMesh();
        void CreateBoxMesh();

        const MeshInfo& GetMeshInfo() const { return m_meshInfo; }

        std::string GetMeshName() const { return m_meshName.c_str(); }

        bool IsInitialize() const { return m_isInitialize; }
    };

} // namespace

#endif