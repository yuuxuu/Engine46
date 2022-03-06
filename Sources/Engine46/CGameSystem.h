/**
 * @file CGameSystem.h
 * @brief
 * @author 木村優
 * @date 2018/12/19
 */

#pragma once

#ifndef _CGAME_SYSTEM_H_
#define _CGAME_SYSTEM_H_

namespace Engine46 {

    // 前方宣言
    class CRendererBase;
    class CActorManager;
    class CShaderManager;
    class CMeshManager;
    class CMaterialManager;
    class CTextureManager;
    class CInput;

    class CGameSystem {
    private:
        std::thread                         m_gameSystemThread;
        HANDLE                              m_hGame;

        HWND                                m_hwnd;

        std::unique_ptr<CActorManager>      m_pActorManager;

        std::unique_ptr<CShaderManager>     m_pShaderManager;

        std::unique_ptr<CMeshManager>       m_pMeshManager;

        std::unique_ptr<CMaterialManager>   m_pMaterialManager;

        std::unique_ptr<CTextureManager>    m_pTextureManager;

        std::unique_ptr<CInput>             m_pInput;

        bool                                m_isInitialize;

    private:
        CGameSystem();
        ~CGameSystem();

        void Loop();
        void Update();

    public:

        bool Initialize(CRendererBase* pRenderer, HWND hwnd);
        void Finalize();

        static CGameSystem& GetGameSystem() {
            static CGameSystem gameSystem;
            return gameSystem;
        };

        CActorManager* GetActorManager() const { return m_pActorManager.get(); }
        CShaderManager* GetShaderManager() const { return m_pShaderManager.get(); }
        CMeshManager* GetMeshManager() const { return m_pMeshManager.get(); }
        CMaterialManager* GetMaterialManager() const { return m_pMaterialManager.get(); }
        CTextureManager* GetTextureManager() const { return m_pTextureManager.get(); }

        HWND GetHwnd() const { return m_hwnd; }

        CInput* GetInput() const { return m_pInput.get(); }

        bool IsInitialize() const { return m_isInitialize; }
    };

} // namespace

#endif
