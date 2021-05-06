/**
 * @file CMesh.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/06
 */

#include "CMesh.h"
#include "../Renderer/CDX11Renderer.h"
#include "../Engine46/utility.h"

namespace Engine46 {

	// コンストラクタ
	CMeshBase::CMeshBase() :
		m_strides(0)
	{}

	// デストラクタ
	CMeshBase::~CMeshBase()
	{
		VecClear(m_vecVertex);
		VecClear(m_vecColor);
		VecClear(m_vecUV);
		VecClear(m_vecNormal);
		VecClear(m_vecBinormal);
		VecClear(m_vecTangent);
	}

	// コンストラクタ
	CDX11Mesh::CDX11Mesh(CDX11Renderer* pRenderer) :
		pRenderer(pRenderer)
	{}

	// デストラクタ
	CDX11Mesh::~CDX11Mesh()
	{}

	// メッシュ作成
	void CDX11Mesh::Create() {

		m_strides = 0;
		if (!m_vecVertex.empty()) {
			m_strides += sizeof(m_vecVertex[0]);
		}
		if (!m_vecColor.empty()) {
			m_strides += sizeof(m_vecColor[0]);
		}
		if (!m_vecUV.empty()) {
			m_strides += sizeof(m_vecUV[0]);
		}
		if (!m_vecNormal.empty()) {
			m_strides += sizeof(m_vecNormal[0]);
		}
		if (!m_vecBinormal.empty()) {
			m_strides += sizeof(m_vecBinormal[0]);
		}
		if (!m_vecTangent.empty()) {
			m_strides += sizeof(m_vecTangent[0]);
		}

		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.ByteWidth			= m_strides * m_vecVertex.size();
		bufDesc.Usage				= D3D11_USAGE_DEFAULT;
		bufDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
		bufDesc.CPUAccessFlags		= 0;
		bufDesc.MiscFlags			= 0;
		bufDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = 0;

		pRenderer->CreateBuffer(m_pVertexBuffer, bufDesc, &subData);

		bufDesc.ByteWidth			= sizeof(m_vecIndexes[0]) * m_vecIndexes.size();
		bufDesc.BindFlags			= D3D11_BIND_INDEX_BUFFER;

		subData.pSysMem = &m_vecIndexes[0];

		pRenderer->CreateBuffer(m_pIndexBuffer, bufDesc, &subData);
	}

	// メッシュ描画
	void CDX11Mesh::Draw() {
		
		pRenderer->SetBuffer(m_pVertexBuffer.GetAddressOf(), m_pIndexBuffer.Get(), m_strides, 0);

		pRenderer->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_vecIndexes.size());
	}

} // namespace