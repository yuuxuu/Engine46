/**
 * @file CDX12Mesh.h
 * @brief
 * @author 木村優
 * @date 2021/08/27
 */

#pragma once

#ifndef _CDX12_MESH_H_
#define _CDX12_MESH_H_

#include "../CMesh.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

    // 前方宣言
    class CDX12Device;
    class CDX12Command;

    class CDX12Mesh : public CMeshBase {
    private:
        CDX12Device* pDX12Device;
        CDX12Command* pDX12Command;

        ComPtr<ID3D12Resource>      m_pVertexResource;
        ComPtr<ID3D12Resource>      m_pIndexResource;

        D3D12_VERTEX_BUFFER_VIEW    m_vbView;
        D3D12_INDEX_BUFFER_VIEW     m_ibView;

    public:
        CDX12Mesh(CDX12Device* pDevice, CDX12Command* pCommand, const std::string& meshName);
        ~CDX12Mesh();

        void CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE type) override;
        void CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE type, const std::vector<VertexInfo>& vecVertexInfo) override;
        void CreateIndexBuffer() override;
        void CreateIndexBuffer(const std::vector<DWORD>& vecIndex) override;
        void Draw() override;
    };
} // namespace

#endif