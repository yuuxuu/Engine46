/**
 * @file CDX11ConstantBuffer.h
 * @brief
 * @author 木村優
 * @date 2021/05/22
 */

#pragma once

#ifndef _CCDX11_CONSTANTBUFFER_H_
#define _CCDX11_CONSTANTBUFFER_H_

#include "../Engine46/CConstantBuffer.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	// 前方宣言
	class CDX11Device;
	class CDX11DeviceContext;

	class CDX11ConstantBuffer : public CConstantBufferBase {
	private:
		CDX11Device*			pDX11Device;
		CDX11DeviceContext*		pDX11DeviceContext;

		ComPtr<ID3D11Buffer>	m_pConstantBuffer;

	public:
		CDX11ConstantBuffer(CDX11Device* pDevice, CDX11DeviceContext* pDeviceContext);
		~CDX11ConstantBuffer();

		void CreateConstantBuffer(UINT byteWidth) override;
		void Update(void* srcData) override;
		void Set(UINT slot) override;
	};
} // namespace

#endif