/**
 * @file CForwardRendering.h
 * @brief
 * @author 木村優
 * @date 2021/05/04
 */

#pragma once

#ifndef _CFORWARDRENDERING_H_
#define _CFORWARDRENDERING_H_

#include "IRendering.h"
#include "CDX11Renderer.h"

namespace Engine46 {

	class CDX11ForwardRendering : public IRendering {
	private:
		CDX11Renderer*						pRenderer;

		ComPtr<ID3D11RenderTargetView>		m_pRtv;

		std::unique_ptr<CDX11Texture>		m_pRtvTex2D;

	public:
		explicit CDX11ForwardRendering(CDX11Renderer* pRenderer);
		~CDX11ForwardRendering();

		bool Initialize(UINT width, UINT height) override;
		void Begine() override;
		void End() override;

		CTextureBase* GetRenderingTexture() const { return m_pRtvTex2D.get(); }
	};

} // namespace

#endif