/**
 * @file CMesh.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/06
 */

#include "CMesh.h"

#include "../Engine46/utility.h"

#include "../Renderer/CDX11Renderer.h"

namespace Engine46 {

	// コンストラクタ
	CMeshBase::CMeshBase()
	{}

	// デストラクタ
	CMeshBase::~CMeshBase()
	{
		VecClear(m_vecVertexInfo);

		VecClear(m_vecIndexes);
	}

	// 頂点配列の初期化
	void CMeshBase::ReserveVertex(int reserveSize) {
		m_vecVertexInfo.reserve(reserveSize);
	}

	// インデックス配列の初期化
	void CMeshBase::ReserveIndex(int reserveSize) {
		m_vecIndexes.reserve(reserveSize);
	}

	// コンストラクタ
	CDX11Mesh::CDX11Mesh(CDX11Renderer* pRenderer) :
		pDX11Renderer(pRenderer)
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

		pDX11Renderer->CreateBuffer(m_pVertexBuffer, bufDesc, &subData);

		bufDesc.ByteWidth			= sizeof(m_vecIndexes[0]) * m_vecIndexes.size();
		bufDesc.BindFlags			= D3D11_BIND_INDEX_BUFFER;

		subData.pSysMem = &m_vecIndexes[0];

		pDX11Renderer->CreateBuffer(m_pIndexBuffer, bufDesc, &subData);
	}

	// メッシュ描画
	void CDX11Mesh::Draw() {
		
		pDX11Renderer->SetBuffer(m_pVertexBuffer.Get(), m_pIndexBuffer.Get(), sizeof(vertexInfo), 0);

		pDX11Renderer->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_vecIndexes.size());
	}

} // namespace