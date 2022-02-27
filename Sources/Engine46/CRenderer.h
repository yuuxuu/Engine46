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
#include "CShaderPackage.h"
#include "CConstantBuffer.h"
#include "CUnorderedAccessBuffer.h"

#include "CActor.h"

namespace Engine46 {

    // 前方宣言
    class CMeshBase;
    class CTextureBase;

    class CRendererBase : public IRenderer {
    protected:
        std::unique_ptr<CRenderingBase>         m_pForwardRendering;
        std::unique_ptr<CRenderingBase>         m_pTiledForwardRendering;
        std::unique_ptr<CRenderingBase>         m_pDeferredRendering;
        std::unique_ptr<CRenderingBase>         m_pDepthRendring;

        RECT                                    m_windowRect;

        std::unique_ptr<CConstantBufferBase>    m_pCameraCB;
        std::unique_ptr<CConstantBufferBase>    m_pDirectionalLightCB;
        std::unique_ptr<CConstantBufferBase>    m_pPointLightCB;
        std::unique_ptr<CConstantBufferBase>    m_pSpotLightCB;
        std::unique_ptr<CConstantBufferBase>    m_pScreenParamCB;

        std::unique_ptr<CActorBase>             m_pRenderSprite;

        CTextureBase*                           pCubeTexture;

    public:
        CRendererBase();
        virtual ~CRendererBase();

        virtual bool Initialize(HWND hwnd, UINT width, UINT height) override { return true; };
        virtual void Finalize() override {};
        virtual void Begine(CSceneBase* pScene) override {};
        virtual bool Render(CSceneBase* pScene) override { return true; };
        virtual void SetSceneConstantBuffers(UINT startSlot) {};
        virtual void SetCameraCb(UINT slot, bool useCompute) {};
        virtual void SetDirectionalLightCb(UINT slot, bool useCompute) {};
        virtual void SetPointLightCb(UINT slot, bool useCompute) {};
        virtual void SetSpotLightCb(UINT slot, bool useCompute) {};
        virtual void SetScreenParamCb(UINT slot, bool useCompute) {};
        virtual void SetCubeTexture(UINT slot) {};

        virtual void CreateConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer, UINT byteWidth) {};
        virtual void CreateUnorderedAccessBuffer(std::unique_ptr<CUnorderedAccessBufferBase>& pUnorderedAccessBuffer, UINT byteWidth, UINT byteSize) {};
        virtual void CreateMesh(std::unique_ptr<CMeshBase>& pMesh, const std::string& meshName) {};
        virtual void CreateTexture(std::unique_ptr<CTextureBase>& pTexture, const std::string& textureName) {};
        virtual void CreateShaderResourceView(CTextureBase* pTexture) {};
        virtual void CreateShader(std::unique_ptr<CShaderPackage>& pShaderPackage, const std::string& shaderName) {};

        RECT GetWindowRect() const { return m_windowRect; }
        CActorBase* GetRenderSprite() const { return m_pRenderSprite.get(); }
    };

} // namespace

#endif