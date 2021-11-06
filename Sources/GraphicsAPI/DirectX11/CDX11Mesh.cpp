/**
 * @file CDX11Mesh.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/22
 */

#include "CDX11Mesh.h"
#include "CDX11Device.h"
#include "CDX11DeviceContext.h"

namespace Engine46 {

    // コンストラクタ
    CDX11Mesh::CDX11Mesh(CDX11Device* pDevice, CDX11DeviceContext* pDeviceContext, const char* meshName) :
        CMeshBase(meshName),
        pDX11Device(pDevice),
        pDX11DeviceContext(pDeviceContext)
    {}

    // デストラクタ
    CDX11Mesh::~CDX11Mesh()
    {}

    // メッシュ作成
    void CDX11Mesh::CreateVertexBuffer(PRIMITIVE_TOPOLOGY_TYPE type) {

        if (m_isInitialize) return;

        if (!m_vecVertexInfo.empty()) {
            D3D11_BUFFER_DESC bufDesc = {};
            bufDesc.ByteWidth = sizeof(m_vecVertexInfo[0]) * (UINT)m_vecVertexInfo.size();
            bufDesc.Usage = D3D11_USAGE_DEFAULT;
            bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bufDesc.CPUAccessFlags = 0;
            bufDesc.MiscFlags = 0;
            bufDesc.StructureByteStride = 0;

            D3D11_SUBRESOURCE_DATA subData = {};
            subData.pSysMem = &m_vecVertexInfo[0];

            pDX11Device->CreateBuffer(m_pVertexBuffer, bufDesc, &subData);

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

            m_isInitialize = true;
        }
    }

    void CDX11Mesh::CreateIndexBuffer() {
        if (!m_vecIndexes.empty()) {
            D3D11_BUFFER_DESC bufDesc = {};
            bufDesc.ByteWidth = sizeof(m_vecIndexes[0]) * (UINT)m_vecIndexes.size();
            bufDesc.Usage = D3D11_USAGE_DEFAULT;
            bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            bufDesc.CPUAccessFlags = 0;
            bufDesc.MiscFlags = 0;
            bufDesc.StructureByteStride = 0;

            D3D11_SUBRESOURCE_DATA subData = {};
            subData.pSysMem = &m_vecIndexes[0];

            pDX11Device->CreateBuffer(m_pIndexBuffer, bufDesc, &subData);
        }
    }

    // メッシュ描画
    void CDX11Mesh::Draw() {

        pDX11DeviceContext->SetBuffer(m_pVertexBuffer.Get(), m_pIndexBuffer.Get(), sizeof(VertexInfo), 0);

        pDX11DeviceContext->DrawIndexed((D3D_PRIMITIVE_TOPOLOGY)m_primitiveTopologyType, (UINT)m_vecIndexes.size());
    }

} // namespace