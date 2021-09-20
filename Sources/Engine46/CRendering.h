/**
 * @file CRendering.h
 * @brief
 * @author 木村優
 * @date 2021/05/23
 */

#pragma once

#ifndef _CRENDERING_H_
#define _CRENDERING_H_

#include "CTexture.h"

namespace Engine46 {

	// 前方宣言
	class CSceneBase;
	class CSprite;

	static const DXGI_FORMAT RENDER_TARGET_FORMATS[] = {
		DXGI_FORMAT_B8G8R8A8_UNORM,		// Diffuse
		DXGI_FORMAT_R11G11B10_FLOAT,	// Specular
		DXGI_FORMAT_R16G16B16A16_FLOAT,	// Normal
		DXGI_FORMAT_R16G16B16A16_FLOAT,	// Position
	};

	const UINT RENDER_TARGET_SIZE = sizeof(RENDER_TARGET_FORMATS) / sizeof(RENDER_TARGET_FORMATS[0]);

	class CRenderingBase {
	protected:
		std::unique_ptr<CTextureBase> m_pRenderTex;

	public:
		CRenderingBase();
		virtual ~CRenderingBase();

		virtual bool Initialize(UINT width, UINT height) { return true; }
		virtual void Begine() {};
		virtual void End() {};

		virtual void Rendering(CSceneBase* pScene) {};
		virtual void DrawForSceneLighting(CSprite* pSprite) {};
		virtual void DrawForRenderScene(CSprite* pSprite, UINT x, UINT y, UINT width, UINT height) {};

		CTextureBase* GetRenderTexture() const { return m_pRenderTex.get(); }
	};

} // namespace

#endif