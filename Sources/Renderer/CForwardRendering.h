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
		ComPtr<ID3D11ShaderResourceView>	m_pSrv;
		ComPtr<ID3D11Texture2D>				m_pTex2D;

	public:
		explicit CDX11ForwardRendering(CDX11Renderer* pRenderer);
		~CDX11ForwardRendering();

		bool Initialize(UINT width, UINT height) override;
		void Begine() override;
		void End() override;
	};

} // namespace

#endif