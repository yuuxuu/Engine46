﻿/**
 * @file CTextureManager.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/23
 */

#include "CTextureManager.h"
#include "CTexture.h"
#include "CActor.h"

#include "CRenderer.h"

namespace Engine46 {

    // コンストラクタ
    CTextureManager::CTextureManager(CRendererBase* pRenderer) :
        pRenderer(pRenderer)
    {}

    // デストラクタ
    CTextureManager::~CTextureManager()
    {}

    // テクスチャを作成
    CTextureBase* CTextureManager::CreateTexture(const std::string& textureName) {
        std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);

        CTextureBase* pTexture = GetTextureFromMap(textureName);

        if (pTexture) return pTexture;

        std::unique_ptr<CTextureBase> texture;

        pRenderer->CreateTexture(texture, textureName);

        if (texture) {
            pTexture = texture.get();

            this->AddTextureToMap(textureName, texture);

            return pTexture;
        }

        return nullptr;
    }

    // テクスチャをマップへ追加
    void CTextureManager::AddTextureToMap(const std::string& name, std::unique_ptr<CTextureBase>& pTexture) {

        if (!GetTextureFromMap(name)) {
            m_mapTexture[name] = std::move(pTexture);
        }
    }

    // テクスチャを取得
    CTextureBase* CTextureManager::GetTextureFromMap(const std::string& name) {
        auto itr = m_mapTexture.find(name);

        if (itr != m_mapTexture.end()) {
            return itr->second.get();
        }

        return nullptr;
    }

} // namespace