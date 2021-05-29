/**
 * @file CRenderer.h
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#pragma once

#ifndef _CRENDERER_H_
#define _CRENDERER_H_

#include "IRenderer.h"
#include "CRendering.h"

#include "math.h"

namespace Engine46 {

	// 前方宣言
	class CActorBase;
	class CMeshManager;
	class CMaterialManager;
	class CShaderManager;
	class CTextureManager;

	class CRendererBase : public IRenderer {
	protected:
		std::unique_ptr<CRenderingBase> m_pRendering;

		RECT							m_windowRect;

	public:
		CRendererBase();
		virtual ~CRendererBase();

		virtual bool Initialize(HWND hwnd, UINT width, UINT height) override { return true; };
		virtual void Finalize() override {};
		virtual bool Render(CSceneBase* pScene) override { return true; };

		virtual void CreateConstantBuffer(CActorBase*& pActor) {};
		virtual void CreateMesh(CMeshManager*& pMeshManager, CActorBase*& pActor) {};
		virtual void CreateMaterial(CMaterialManager*& pMeshManager, CActorBase*& pActor) {};
		virtual void CreateTexture(CTextureManager*& pTextureManager, const char* textureName) {};
		virtual void CreateShader(CShaderManager*& pShaderManager, const char* shaderName) {};

		RECT GetWindowRect() const { return m_windowRect; }
	};

} // namespace

#endif