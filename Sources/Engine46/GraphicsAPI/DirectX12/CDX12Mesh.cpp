﻿/**
 * @file CDX12Mesh.cpp
 * @brief
 * @author 木村優
 * @date 2021/08/27
 */

#include "CDX12Mesh.h"

#include "CDX12Device.h"
#include "CDX12Command.h"
#include "CDX12ShaderPackage.h"

namespace Engine46 {

    // コンストラクタ
    CDX12Mesh::CDX12Mesh(CDX12Device* pDevice, CDX12Command* pCommand, const char* meshName) :
        CMeshBase(meshName),
        pDX12Device(pDevice),
        pDX12Command(pCommand)
    {}

    // デストラクタ
    CDX12Mesh::~CDX12Mesh()
    {}

    // メッシュ作成
    void CDX12Mesh::CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE type) {

        if (m_vecVertexInfo.empty()) return;

        UINT size = sizeof(m_vecVertexInfo[0]) * (UINT)m_vecVertexInfo.size();

        D3D12_RESOURCE_DESC rDesc = {};

        rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        rDesc.Width = size;
        rDesc.Height = 1;
        rDesc.DepthOrArraySize = 1;
        rDesc.MipLevels = 1;
        rDesc.Format = DXGI_FORMAT_UNKNOWN;
        rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        rDesc.SampleDesc.Count = 1;

        CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        pDX12Device->CreateResource(m_pVertexResource, prop, rDesc);

        LPVOID pBuf;
        D3D12_RANGE range = { 0, 0 };

        HRESULT hr = m_pVertexResource->Map(0, &range, &pBuf);
        if (FAILED(hr)) {
            m_pVertexResource->Unmap(0, nullptr);
        }
        else {
            memcpy(pBuf, &m_vecVertexInfo[0], size);

            m_pVertexResource->Unmap(0, nullptr);
        }

        m_vbView.BufferLocation = m_pVertexResource->GetGPUVirtualAddress();
        m_vbView.SizeInBytes = size;
        m_vbView.StrideInBytes = sizeof(m_vecVertexInfo[0]);

        switch (type) {
        case PRIMITIVE_TOPOLOGY_TYPE::POINTLIST:
            m_primitiveTopologyType = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
            break;
        case PRIMITIVE_TOPOLOGY_TYPE::LINELIST:
            m_primitiveTopologyType = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
            break;
        case PRIMITIVE_TOPOLOGY_TYPE::LINESTRIP:
            m_primitiveTopologyType = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
            break;
        case PRIMITIVE_TOPOLOGY_TYPE::TRIANGLELIST:
            m_primitiveTopologyType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            break;
        case PRIMITIVE_TOPOLOGY_TYPE::TRIANGLESTRIP:
            m_primitiveTopologyType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
            break;
        }
    }

    void CDX12Mesh::CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE type, const std::vector<VertexInfo>& vecVertexInfo) {
        
        if (vecVertexInfo.empty()) return;

        m_vecVertexInfo = vecVertexInfo;
        
        UINT size = sizeof(m_vecVertexInfo[0]) * (UINT)m_vecVertexInfo.size();

        D3D12_RESOURCE_DESC rDesc = {};

        rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        rDesc.Width = size;
        rDesc.Height = 1;
        rDesc.DepthOrArraySize = 1;
        rDesc.MipLevels = 1;
        rDesc.Format = DXGI_FORMAT_UNKNOWN;
        rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        rDesc.SampleDesc.Count = 1;

        CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        pDX12Device->CreateResource(m_pVertexResource, prop, rDesc);

        LPVOID pBuf;
        D3D12_RANGE range = { 0, 0 };

        HRESULT hr = m_pVertexResource->Map(0, &range, &pBuf);
        if (FAILED(hr)) {
            m_pVertexResource->Unmap(0, nullptr);
        }
        else {
            memcpy(pBuf, &m_vecVertexInfo[0], size);

            m_pVertexResource->Unmap(0, nullptr);
        }

        m_vbView.BufferLocation = m_pVertexResource->GetGPUVirtualAddress();
        m_vbView.SizeInBytes = size;
        m_vbView.StrideInBytes = sizeof(m_vecVertexInfo[0]);

        switch (type) {
        case PRIMITIVE_TOPOLOGY_TYPE::POINTLIST:
            m_primitiveTopologyType = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
            break;
        case PRIMITIVE_TOPOLOGY_TYPE::LINELIST:
            m_primitiveTopologyType = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
            break;
        case PRIMITIVE_TOPOLOGY_TYPE::LINESTRIP:
            m_primitiveTopologyType = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
            break;
        case PRIMITIVE_TOPOLOGY_TYPE::TRIANGLELIST:
            m_primitiveTopologyType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            break;
        case PRIMITIVE_TOPOLOGY_TYPE::TRIANGLESTRIP:
            m_primitiveTopologyType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
            break;
        }
    }

    void CDX12Mesh::CreateIndexBuffer() {
        
        if (m_vecIndex.empty()) return;

        UINT size = sizeof(m_vecIndex[0]) * (UINT)m_vecIndex.size();

        D3D12_RESOURCE_DESC rDesc = {};

        rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        rDesc.Width = size;
        rDesc.Height = 1;
        rDesc.DepthOrArraySize = 1;
        rDesc.MipLevels = 1;
        rDesc.Format = DXGI_FORMAT_UNKNOWN;
        rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        rDesc.SampleDesc.Count = 1;

        CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        pDX12Device->CreateResource(m_pIndexResource, prop, rDesc);

        LPVOID pBuf;
        D3D12_RANGE range = { 0, 0 };

        HRESULT hr = m_pIndexResource->Map(0, &range, &pBuf);
        if (FAILED(hr)) {
            m_pIndexResource->Unmap(0, nullptr);
        }
        else {
            memcpy(pBuf, &m_vecIndex[0], size);

            m_pIndexResource->Unmap(0, nullptr);
        }

        m_ibView.BufferLocation = m_pIndexResource->GetGPUVirtualAddress();
        m_ibView.SizeInBytes = size;
        m_ibView.Format = DXGI_FORMAT_R32_UINT;
    }

    void CDX12Mesh::CreateIndexBuffer(const std::vector<DWORD>& vecIndex) {
        
        if (vecIndex.empty()) return;

        m_vecIndex = vecIndex;
        
        UINT size = sizeof(m_vecIndex[0]) * (UINT)m_vecIndex.size();

        D3D12_RESOURCE_DESC rDesc = {};

        rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        rDesc.Width = size;
        rDesc.Height = 1;
        rDesc.DepthOrArraySize = 1;
        rDesc.MipLevels = 1;
        rDesc.Format = DXGI_FORMAT_UNKNOWN;
        rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        rDesc.SampleDesc.Count = 1;

        CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        pDX12Device->CreateResource(m_pIndexResource, prop, rDesc);

        LPVOID pBuf;
        D3D12_RANGE range = { 0, 0 };

        HRESULT hr = m_pIndexResource->Map(0, &range, &pBuf);
        if (FAILED(hr)) {
            m_pIndexResource->Unmap(0, nullptr);
        }
        else {
            memcpy(pBuf, &m_vecIndex[0], size);

            m_pIndexResource->Unmap(0, nullptr);
        }

        m_ibView.BufferLocation = m_pIndexResource->GetGPUVirtualAddress();
        m_ibView.SizeInBytes = size;
        m_ibView.Format = DXGI_FORMAT_R32_UINT;
    }

    // メッシュ描画
    void CDX12Mesh::Draw() {

        pDX12Command->SetBuffer(m_vbView, m_ibView);

        if (m_ibView.SizeInBytes != 0) {
            pDX12Command->DrawIndexed((D3D12_PRIMITIVE_TOPOLOGY)m_primitiveTopologyType, (UINT)m_vecIndex.size());
        }
        else {
            pDX12Command->DrawInstanced((D3D12_PRIMITIVE_TOPOLOGY)m_primitiveTopologyType, (UINT)m_vecVertexInfo.size());
        }
    }

} // namespace