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
	CDX11Mesh::CDX11Mesh(CDX11Device* pDevice, CDX11DeviceContext* pDeviceContext) :
		pDX11Device(pDevice),
		pDX11DeviceContext(pDeviceContext)
	{}

	// デストラクタ
	CDX11Mesh::~CDX11Mesh()
	{}

	// メッシュ作成
	void CDX11Mesh::Create() {

		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.ByteWidth			= sizeof(vertexInfo) * m_vecVertexInfo.size();
		bufDesc.Usage				= D3D11_USAGE_DEFAULT;
		bufDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
		bufDesc.CPUAccessFlags		= 0;
		bufDesc.MiscFlags			= 0;
		bufDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = &m_vecVertexInfo[0];

		pDX11Device->CreateBuffer(m_pVertexBuffer, bufDesc, &subData);

		bufDesc.ByteWidth = sizeof(m_vecIndexes[0]) * m_vecIndexes.size();
		bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		subData.pSysMem = &m_vecIndexes[0];

		pDX11Device->CreateBuffer(m_pIndexBuffer, bufDesc, &subData);
	}

	// メッシュ描画
	void CDX11Mesh::Draw() {

		pDX11DeviceContext->SetBuffer(m_pVertexBuffer.Get(), m_pIndexBuffer.Get(), sizeof(vertexInfo), 0);

		pDX11DeviceContext->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_vecIndexes.size());
	}

} // namespace