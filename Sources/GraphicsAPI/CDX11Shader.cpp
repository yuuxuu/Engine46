/**
 * @file CDX11Shader.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/22
 */

#include "CDX11Shader.h"
#include "CDX11Device.h"
#include "CDX11DeviceContext.h"

namespace Engine46 {

	// コンストラクタ
	CDX11Shader::CDX11Shader(CDX11Device* pDevice, CDX11DeviceContext* pDeviceContext, const char* name, ComPtr<ID3DBlob>& pBlob, SHADER_TYPE type) :
		CShader(name, pBlob, type),
		pDX11Device(pDevice),
		pDX11DeviceContext(pDeviceContext)
	{}

	// デストラクタ
	CDX11Shader::~CDX11Shader()
	{}

	// シェーダーを作成
	void CDX11Shader::Create() {
		switch (m_shaderType) {
		case SHADER_TYPE::TYPE_VERTEX:
			pDX11Device->CreateVertexShader(m_pVS, m_pBlob.Get());
			break;
		case SHADER_TYPE::TYPE_PIXEL:
			pDX11Device->CreatePixelShader(m_pPS, m_pBlob.Get());
			break;
		case SHADER_TYPE::TYPE_HULL:
			pDX11Device->CreateHullShader(m_pHS, m_pBlob.Get());
			break;
		case SHADER_TYPE::TYPE_DOMAIN:
			pDX11Device->CreateDomainShader(m_pDS, m_pBlob.Get());
			break;
		case SHADER_TYPE::TYPE_GEOMETRY:
			pDX11Device->CreateGeometryShader(m_pGS, m_pBlob.Get());
			break;
		case SHADER_TYPE::TYPE_COMPUTE:
			pDX11Device->CreateComputeShader(m_pCS, m_pBlob.Get());
			break;
		}
	}

	// シェーダーを設定
	void CDX11Shader::Set() {
		switch (m_shaderType) {
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

} // namespace