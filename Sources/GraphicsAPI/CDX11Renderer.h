/**
 * @file CDX11Renderer.h
 * @brief
 * @author 木村優
 * @date 2021/05/02
 */

#pragma once

#ifndef _CDX11RENDERER_H_
#define _CDX11RENDERER_H_

#include "IRenderer.h"

#include "CDX11Device.h"
#include "CDX11DeviceContext.h"

#include "../Engine46/math.h"

namespace Engine46 {

	// 前方宣言
	class CDX11ForwardRendering;

	class CDX11Renderer : public IRenderer {
	private:
		std::unique_ptr<CDX11ForwardRendering>	m_pDX11FRendering;

		RECT									m_windowRect;

		std::unique_ptr<CDX11Device>			m_pDX11Device;
		std::unique_ptr<CDX11DeviceContext>		m_pDX11DeviceContext;

		ComPtr<ID3D11RenderTargetView>			m_pRtv;

		ComPtr<ID3D11DepthStencilView>			m_pDsv;

	public:
		CDX11Renderer();
		~CDX11Renderer();

		bool Initialize(HWND hwnd, UINT width, UINT height) override;
		
		void Finalize() override;

		bool Render(CSceneBase* pScene) override;

		CDX11Device* GetDX11Device() const { return m_pDX11Device.get(); }

		CDX11DeviceContext* GetDX11DeviceContext() const { return m_pDX11DeviceContext.get(); }

		RECT GetWindowRect() const { return m_windowRect; }
	};

} // namespace

#endif