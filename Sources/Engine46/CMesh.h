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

    struct vertexInfo {
        VECTOR3	vertex;
        VECTOR4	color;
        VECTOR2	uv;
        VECTOR3	normal;
        VECTOR3	tangent;
        VECTOR3	binormal;
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
        std::vector<vertexInfo>	m_vecVertexInfo;

        std::vector<DWORD>      m_vecIndexes;

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

        void ReserveVertex(int reserveSize);
        void ReserveIndex(int reserveSize);

        void AddVertexInfo(vertexInfo info) { m_vecVertexInfo.emplace_back(info); }

        void AddIndex(const DWORD index) { m_vecIndexes.emplace_back(index); }

        std::string GetMeshName() const { return m_meshName.c_str(); }

        bool IsInitialize() const { return m_isInitialize; }
    };

} // namespace

#endif