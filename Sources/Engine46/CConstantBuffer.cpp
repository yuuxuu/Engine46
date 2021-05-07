/**
 * @file CConstantBuffer.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#include "CConstantBuffer.h"

#include "../Renderer/CDX11Renderer.h"

namespace Engine46 {

	// コンストラクタ
	CCbBase::CCbBase()
	{}

	// デストラクタ
	CCbBase::~CCbBase()
	{}

	// コンストラクタ
	CDX11CB::CDX11CB(CDX11Renderer* pRenderer) :
		pDX11Renderer(pRenderer)
	{}

	// デストラクタ
	CDX11CB::~CDX11CB()
	{}

	// コンスタントバッファ作成
	void CDX11CB::CreateConstantBuffer(UINT byteWidth) {
		
		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.ByteWidth			= byteWidth;
		bufDesc.Usage				= D3D11_USAGE_DEFAULT;
		bufDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
		bufDesc.CPUAccessFlags		= 0;
		bufDesc.MiscFlags			= 0;
		bufDesc.StructureByteStride = 0;

		pDX11Renderer->CreateBuffer(m_pConstantBuffer, bufDesc);
	}

	// 更新
	void CDX11CB::Update(void* srcData) {
		pDX11Renderer->UpdateSubResource(m_pConstantBuffer.Get(), srcData);
	}

	// コンスタントバッファをシェーダーへ設定
	void CDX11CB::Set(UINT slot) {
		pDX11Renderer->SetPSConstantBuffers(slot, 1, m_pConstantBuffer.Get());
	}

} // namespace