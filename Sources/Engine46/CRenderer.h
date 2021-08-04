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
#include "math.h"
#include "CRendering.h"
#include "CShaderPackage.h"
#include "CConstantBuffer.h"

namespace Engine46 {

	// 前方宣言
	class CMeshBase;
	class CMaterialBase;
	class CTextureBase;

	class CRendererBase : public IRenderer {
	protected:
		std::unique_ptr<CRenderingBase>			m_pRendering;

		RECT									m_windowRect;

		std::unique_ptr<CConstantBufferBase>	m_pDirectionalLightCB;
		std::unique_ptr<CConstantBufferBase>	m_pPointLightCB;
		std::unique_ptr<CConstantBufferBase>	m_pSpotLightCB;

	public:
		CRendererBase();
		virtual ~CRendererBase();

		virtual bool Initialize(HWND hwnd, UINT width, UINT height) override { return true; };
		virtual void Finalize() override {};
		virtual void Begine(CSceneBase* pScene) override {};
		virtual bool Render(CSceneBase* pScene) override { return true; };

		virtual void CreateConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer) {};
		virtual void CreateMesh(std::unique_ptr<CMeshBase>& pMesh) {};
		virtual void CreateMaterial(std::unique_ptr<CMaterialBase>& pMaterial) {};
		virtual void CreateTexture(std::unique_ptr<CTextureBase>& pTexture, const char* textureName) {};
		virtual void CreateShader(std::unique_ptr<CShaderPackage>& pShaderPackage, const char* shaderName) {};

		RECT GetWindowRect() const { return m_windowRect; }
	};

} // namespace

#endif