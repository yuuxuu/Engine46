/**
 * @file CDX11ShaderPackage.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/22
 */

#include "CDX11ShaderPackage.h"
#include "CDX11Device.h"
#include "CDX11DeviceContext.h"

namespace Engine46 {

    // コンストラクタ
    CDX11ShaderPackage::CDX11ShaderPackage(CDX11Device* pDevice, CDX11DeviceContext* pDeviceContext, const std::string& shaderName) :
        CShaderPackage(shaderName),
        pDX11Device(pDevice),
        pDX11DeviceContext(pDeviceContext)
    {}

    // デストラクタ
    CDX11ShaderPackage::~CDX11ShaderPackage()
    {}

    // シェーダーを作成
    bool CDX11ShaderPackage::Initialize() {

        for (const auto& shader : m_pVecShader) {
            switch (shader->GetShaderType()) {
            case SHADER_TYPE::TYPE_VERTEX:
                pDX11Device->CreateVertexShader(m_pVS, shader->GetBlob());
                break;
            case SHADER_TYPE::TYPE_PIXEL:
                pDX11Device->CreatePixelShader(m_pPS, shader->GetBlob());
                break;
            case SHADER_TYPE::TYPE_HULL:
                pDX11Device->CreateHullShader(m_pHS, shader->GetBlob());
                break;
            case SHADER_TYPE::TYPE_DOMAIN:
                pDX11Device->CreateDomainShader(m_pDS, shader->GetBlob());
                break;
            case SHADER_TYPE::TYPE_GEOMETRY:
                pDX11Device->CreateGeometryShader(m_pGS, shader->GetBlob());
                break;
            case SHADER_TYPE::TYPE_COMPUTE:
                pDX11Device->CreateComputeShader(m_pCS, shader->GetBlob());
                break;
            }
        }

        return true;
    }

    // シェーダーを設定
    void CDX11ShaderPackage::SetShader() {
        for (const auto& shader : m_pVecShader) {
            switch (shader->GetShaderType()) {
            case SHADER_TYPE::TYPE_VERTEX:
                pDX11DeviceContext->SetVsShader(m_pVS.Get());
                break;
            case SHADER_TYPE::TYPE_PIXEL:
                pDX11DeviceContext->SetPsShader(m_pPS.Get());
                break;
            case SHADER_TYPE::TYPE_HULL:
                pDX11DeviceContext->SetHsShader(m_pHS.Get());
                break;
            case SHADER_TYPE::TYPE_DOMAIN:
                pDX11DeviceContext->SetDsShader(m_pDS.Get());
                break;
            case SHADER_TYPE::TYPE_GEOMETRY:
                pDX11DeviceContext->SetGsShader(m_pGS.Get());
                break;
            case SHADER_TYPE::TYPE_COMPUTE:
                pDX11DeviceContext->SetCsShader(m_pCS.Get());
                break;
            }
        }
    }

} // namespace