/**
 * @file CDX11ConstantBuffer.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/22
 */

#include "CDX11ConstantBuffer.h"
#include "CDX11Device.h"
#include "CDX11DeviceContext.h"

namespace Engine46 {

    // コンストラクタ
    CDX11ConstantBuffer::CDX11ConstantBuffer(CDX11Device* pDevice, CDX11DeviceContext* pDeviceContext) :
        pDX11Device(pDevice),
        pDX11DeviceContext(pDeviceContext)
    {}

    // デストラクタ
    CDX11ConstantBuffer::~CDX11ConstantBuffer()
    {}

    // コンスタントバッファ作成
    void CDX11ConstantBuffer::CreateConstantBuffer(UINT byteWidth) {

        D3D11_BUFFER_DESC bufDesc = {};
        bufDesc.ByteWidth = byteWidth;
        bufDesc.Usage = D3D11_USAGE_DEFAULT;
        bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufDesc.CPUAccessFlags = 0;
        bufDesc.MiscFlags = 0;
        bufDesc.StructureByteStride = 0;

        pDX11Device->CreateBuffer(m_pConstantBuffer, bufDesc);
    }

    // 更新
    void CDX11ConstantBuffer::Update(void* srcData) {
        pDX11DeviceContext->UpdateSubResource(m_pConstantBuffer.Get(), srcData);
    }

    // コンスタントバッファをシェーダーへ設定
    void CDX11ConstantBuffer::Set(UINT slot) {
        pDX11DeviceContext->SetVSConstantBuffers(slot, 1, m_pConstantBuffer.Get());
        pDX11DeviceContext->SetPSConstantBuffers(slot, 1, m_pConstantBuffer.Get());
    }

} // namespace