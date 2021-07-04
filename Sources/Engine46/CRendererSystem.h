/**
 * @file CRendererSystem.h
 * @brief
 * @author 木村優
 * @date 2021/06/20
 */

#pragma once

#ifndef _CRENDERER_SYSTEM_H_
#define _CRENDERER_SYSTEM_H_

#include "math.h"

#pragma comment(lib, "GraphicsAPI.lib")

namespace Engine46 {

	// 前方宣言
	class CRendererBase;
	class CSceneBase;

	class CRendererSystem{
	private:
		std::thread						m_rendererSystemThread;
		HANDLE							m_hRenderer;

		std::unique_ptr<CRendererBase>	m_pRenderer;

		CSceneBase*						pRenderScene;

	private:
		CRendererSystem();
		~CRendererSystem();

		void Loop();
		void Draw();

	public:
		bool Initialize(HWND hwnd, RECT rect);
		void Finalize();

		static CRendererSystem& GetRendererSystem() {
			static CRendererSystem rendererSystem;
			return rendererSystem;
		}

		CRendererBase* GetRenderer() const { return m_pRenderer.get(); }

		void SetRenderScene(CSceneBase* pScene) { pRenderScene = pScene; }
		CSceneBase* GetRenderScene() const { return pRenderScene; }
	};

} // namespace

#endif