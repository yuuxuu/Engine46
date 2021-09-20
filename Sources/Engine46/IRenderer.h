/**
 * @file IRenderer.h
 * @brief
 * @author 木村優
 * @date 2021/05/02
 */

#pragma once

#ifndef _IRENDERER_H_
#define _IRENDERER_H_

#include "CScene.h"

namespace Engine46 {

	class IRenderer {
	protected:
		IRenderer() {};
		virtual ~IRenderer() {};

		virtual bool Initialize(HWND hwnd, UINT width, UINT height) = 0;
		
		virtual void Finalize() = 0;

		virtual void Begine(CSceneBase* pScene) = 0;

		virtual bool Render(CSceneBase* pScene) = 0;
	};

} // namespace

#endif