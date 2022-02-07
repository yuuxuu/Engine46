/**
 * @file CDX11ShaderPackage.h
 * @brief
 * @author 木村優
 * @date 2021/05/22
 */

#pragma once

#ifndef _CDX11_SHADER_PACKAGE_H_
#define _CDX11_SHADER_PACKAGE_H_

#include "../CShaderPackage.h"

namespace Engine46 {

    // 前方宣言
    class CDX11Device;
    class CDX11DeviceContext;

    class CDX11ShaderPackage : public CShaderPackage {
    private:
        CDX11Device* pDX11Device;
        CDX11DeviceContext* pDX11DeviceContext;

        ComPtr<ID3D11VertexShader>      m_pVS;
        ComPtr<ID3D11PixelShader>       m_pPS;
        ComPtr<ID3D11HullShader>        m_pHS;
        ComPtr<ID3D11DomainShader>      m_pDS;
        ComPtr<ID3D11GeometryShader>    m_pGS;
        ComPtr<ID3D11ComputeShader>     m_pCS;

    public:
        CDX11ShaderPackage(CDX11Device* pDevice, CDX11DeviceContext* pDeviceContext, const std::string& shaderName);
        ~CDX11ShaderPackage();

        bool Initialize() override;

        void SetShader() override;
    };
} // namespace

#endif