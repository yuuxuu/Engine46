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

	struct vertexInfo {
		VECTOR3	vertex;
		VECTOR4	color;
		VECTOR2	uv;
		VECTOR3	normal;
		VECTOR3	tangent;
		VECTOR3	binormal;
	};

	// コンストラクタ
	CMeshBase::CMeshBase()
	{}

	// デストラクタ
	CMeshBase::~CMeshBase()
	{
		VecClear(m_vecVertex);
		VecClear(m_vecColor);
		VecClear(m_vecUV);
		VecClear(m_vecNormal);
		VecClear(m_vecTangent);
		VecClear(m_vecBinormal);

		VecClear(m_vecIndexes);
	}

	// 頂点配列の初期化
	void CMeshBase::ReserveVertex(int reserveSize) {
		m_vecVertex.reserve(reserveSize);
		m_vecColor.reserve(reserveSize);
		m_vecUV.reserve(reserveSize);
		m_vecNormal.reserve(reserveSize);
		m_vecTangent.reserve(reserveSize);
		m_vecBinormal.reserve(reserveSize);
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
		bufDesc.ByteWidth			= sizeof(vertexInfo) * m_vecVertex.size();
		bufDesc.Usage				= D3D11_USAGE_DEFAULT;
		bufDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
		bufDesc.CPUAccessFlags		= 0;
		bufDesc.MiscFlags			= 0;
		bufDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = this;

		pDX11Renderer->CreateBuffer(m_pVertexBuffer, bufDesc, &subData);

		bufDesc.ByteWidth			= sizeof(m_vecIndexes[0]) * m_vecIndexes.size();
		bufDesc.BindFlags			= D3D11_BIND_INDEX_BUFFER;

		subData.pSysMem = &m_vecIndexes[0];

		pDX11Renderer->CreateBuffer(m_pIndexBuffer, bufDesc, &subData);
	}

	// メッシュ描画
	void CDX11Mesh::Draw() {
		
		pDX11Renderer->SetBuffer(m_pVertexBuffer.GetAddressOf(), m_pIndexBuffer.Get(), sizeof(vertexInfo), 0);

		pDX11Renderer->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_vecIndexes.size());
	}

} // namespace