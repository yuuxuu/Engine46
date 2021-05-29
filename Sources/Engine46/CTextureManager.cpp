/**
 * @file CTextureManager.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/23
 */

#include "CTextureManager.h"
#include "CTexture.h"
#include "CRenderer.h"

namespace Engine46 {

	const char* g_Texture = "D:/PICTURE/キングダムハーツ/EWmO72SUwAEFtsQ.jpg";

	// コンストラクタ
	CTextureManager::CTextureManager(CRendererBase* pRenderer) :
		pRenderer(pRenderer)
	{}

	// デストラクタ
	CTextureManager::~CTextureManager()
	{}

	// 初期化
	bool CTextureManager::Initialize() {

		CTextureManager* pTextureManager = this;
		pRenderer->CreateTexture(pTextureManager, g_Texture);

		return true;
	}

	// テクスチャを作成
	CTextureBase* CTextureManager::CreateTexture(const char* textureName) {
		auto itr = m_mapTexture.find(textureName);

		if (itr == m_mapTexture.end()) {
			std::unique_ptr<CTextureBase> texture = std::make_unique<CTextureBase>();

			CTextureBase* pTexture = texture.get();

			this->AddTextureToMap(textureName, texture);

			return pTexture;
		}

		return itr->second.get();
	}

	// テクスチャをマップへ追加
	void CTextureManager::AddTextureToMap(const char* name, std::unique_ptr<CTextureBase>& pTexture) {
		auto itr = m_mapTexture.find(name);

		if (itr == m_mapTexture.end()) {
			m_mapTexture[name] = std::move(pTexture);
		}
	}

	// テクスチャを取得
	CTextureBase* CTextureManager::GetTexture(const char* name) {
		auto itr = m_mapTexture.find(name);

		if (itr != m_mapTexture.end()) {
			return itr->second.get();
		}

		return nullptr;
	}

} // namespace