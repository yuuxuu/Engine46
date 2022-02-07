/**
 * @file CTextureManager.h
 * @brief
 * @author 木村優
 * @date 2021/05/23
 */

#pragma once

#ifndef _CTEXTURE_MANAGER_H_
#define _CTEXTURE_MANAGER_H_

namespace Engine46 {

    // 前方宣言
    class CRendererBase;
    class CTextureBase;

    class CTextureManager {
    private:
        std::map<std::string, std::unique_ptr<CTextureBase>> m_mapTexture;

        CRendererBase* pRenderer;

    public:
        explicit CTextureManager(CRendererBase* pRenderer);
        ~CTextureManager();

        CTextureBase* CreateTexture(const std::string& textureName);

        void AddTextureToMap(const std::string&, std::unique_ptr<CTextureBase>& pTexture);

        CTextureBase* GetTextureFromMap(const std::string& name);
    };

} // namespace

#endif