/**
 * @file CDX11Shader.h
 * @brief
 * @author 木村優
 * @date 2021/05/22
 */

#pragma once

#ifndef _CDX11_SHADER_
#define _CDX11_SHADER_

#include "../Engine46/CShader.h"

namespace Engine46 {

	// 前方宣言
	class CDX11Device;
	class CDX11DeviceContext;

	class CDX11Shader : public CShaderBase {
	private:
		CDX11Device*					pDX11Device;
		CDX11DeviceContext*				pDX11DeviceContext;

		ComPtr<ID3D11VertexShader>		m_pVS;
		ComPtr<ID3D11PixelShader>		m_pPS;
		ComPtr<ID3D11HullShader>		m_pHS;
		ComPtr<ID3D11DomainShader>		m_pDS;
		ComPtr<ID3D11GeometryShader>	m_pGS;
		ComPtr<ID3D11ComputeShader>		m_pCS;

	public:
		CDX11Shader(CDX11Device* pDevice, CDX11DeviceContext* pDeviceContext, const char* name, ComPtr<ID3DBlob>& pBlob, SHADER_TYPE type);
		~CDX11Shader();

		void Create() override;
		void Set() override;
	};
} // namespace

#endif