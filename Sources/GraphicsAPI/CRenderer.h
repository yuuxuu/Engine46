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

#include "Engine46/math.h"
#include "Engine46/CShaderPackage.h"
#include "Engine46/CConstantBuffer.h"
#include "Engine46/CSprite.h"

namespace Engine46 {

    // 前方宣言
    class CMeshBase;
    class CMaterialBase;
    class CTextureBase;

    class CRendererBase : public IRenderer {
    protected:
        std::unique_ptr<CRenderingBase>         m_pForwardRendering;
        std::unique_ptr<CRenderingBase>         m_pDeferredRendering;
        std::unique_ptr<CRenderingBase>         m_pDepthRendring;

        RECT                                    m_windowRect;

        std::unique_ptr<CConstantBufferBase>    m_pCameraCB;
        std::unique_ptr<CConstantBufferBase>    m_pDirectionalLightCB;
        std::unique_ptr<CConstantBufferBase>    m_pPointLightCB;
        std::unique_ptr<CConstantBufferBase>    m_pSpotLightCB;

        std::unique_ptr<CSprite>                m_pRenderSprite;

    public:
        CRendererBase();
        virtual ~CRendererBase();

        virtual bool Initialize(HWND hwnd, UINT width, UINT height) override { return true; };
        virtual void Finalize() override {};
        virtual void Begine(CSceneBase* pScene) override {};
        virtual bool Render(CSceneBase* pScene) override { return true; };

        virtual void CreateConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer, UINT byteWidth) {};
        virtual void CreateMesh(std::unique_ptr<CMeshBase>& pMesh, const char* meshName) {};
        virtual void CreateTexture(std::unique_ptr<CTextureBase>& pTexture, const char* textureName) {};
        virtual void CreateShader(std::unique_ptr<CShaderPackage>& pShaderPackage, const char* shaderName) {};

        RECT GetWindowRect() const { return m_windowRect; }
        CSprite* GetRenderSprite() const { return m_pRenderSprite.get(); }
    };

} // namespace

#endif