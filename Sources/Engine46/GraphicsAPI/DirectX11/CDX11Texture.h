/**
 * @file CDX11Texture.h
 * @brief
 * @author 木村優
 * @date 2021/05/22
 */

#pragma once

#ifndef _CDX11_TEXTURE_H_
#define _CDX11_TEXTURE_H_

#include "../CTexture.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

    // 前方宣言
    class CDX11Device;
    class CDX11DeviceContext;

    class CDX11Texture : public CTextureBase {
    private:
        CDX11Device* pDX11Device;
        CDX11DeviceContext* pDX11DeviceContext;

        ComPtr<ID3D11Texture2D>             m_pTex2D;
        ComPtr<ID3D11ShaderResourceView>    m_pSrv;

    public:
        CDX11Texture(CDX11Device* pDevice, CDX11DeviceContext* pDeviceContext, const char* name = "");
        ~CDX11Texture();

        void CreateTexture() override;
        void CreateShaderResourceView() override;
        void Set(UINT slot) override;

        void CreateTexture(D3D11_TEXTURE2D_DESC& texDesc);

        ID3D11Texture2D* GetTexture2D() const { return m_pTex2D.Get(); }
    };

} // namespace

#endif