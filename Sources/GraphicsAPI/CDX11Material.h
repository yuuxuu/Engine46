/**
 * @file CDX11Material.h
 * @brief
 * @author 木村優
 * @date 2021/05/22
 */

#pragma once

#ifndef _CDX11_MATERIAL_H_
#define _CDX11_MATERIAL_H_

#include "../Engine46/CMaterial.h"

namespace Engine46 {

	// 前方宣言
	class CDX11Device;
	class CDX11DeviceContext;
	class CConstantBufferBase;

	class CDX11Material : public CMaterialBase {
	private:
		CDX11Device*							pDX11Device;
		CDX11DeviceContext*						pDX11DeviceContext;

		std::unique_ptr<CConstantBufferBase>	m_pMaterialConstantBuffer;

	public:
		CDX11Material(CDX11Device* pDevice, CDX11DeviceContext* pDeviceContext);
		~CDX11Material();

		void Create() override;
		void Update() override;
		void Set(UINT slot) override;
	};

} // namespace

#endif