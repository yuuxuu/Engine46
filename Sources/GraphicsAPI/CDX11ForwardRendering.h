/**
 * @file CDX11ForwardRendering.h
 * @brief
 * @author 木村優
 * @date 2021/05/04
 */

#pragma once

#ifndef _CDX11_FORWARDRENDERING_H_
#define _CDX11_FORWARDRENDERING_H_

#include "../Engine46/CRendering.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	// 前方宣言
	class CDX11Device;
	class CDX11DeviceContext;

	class CDX11ForwardRendering : public CRenderingBase {
	private:
		CDX11Device*					pDX11Device;
		CDX11DeviceContext*				pDX11DeviceContext;

		ComPtr<ID3D11RenderTargetView>	m_pRtv;
		ComPtr<ID3D11DepthStencilView>	m_pDsv;

	public:
		CDX11ForwardRendering(CDX11Device* pDX11Device, CDX11DeviceContext* pDX11DeviceContext);
		~CDX11ForwardRendering();

		bool Initialize(UINT width, UINT height) override;
		void Begine() override;
		void End() override;

		void Rendering(CSceneBase* pScene) override;
		void DrawForRenderScene(CSprite* pSprite, UINT x, UINT y, UINT width, UINT height) override;
	};

} // namespace

#endif