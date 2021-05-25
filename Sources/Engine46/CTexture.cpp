/**
 * @file CTexture.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#include "CTexture.h"
#include "utility.h"

namespace Engine46 {

	// コンストラクタ
	CTextureBase::CTextureBase() :
		m_textureName()
	{}

	// デストラクタ
	CTextureBase::~CTextureBase()
	{}

	// 初期化
	bool CTextureBase::Initialize(const char* name) {

		m_textureName = name;

		return true;
	}

	// テクスチャを読み込む
	bool CTextureBase::LoadTexture(const char* name) {

		std::unique_ptr<wchar_t[]> loadName;
		CharConvertToWchar(loadName, name);

		DirectX::ScratchImage sImage;
		HRESULT hr = DirectX::LoadFromWICFile(loadName.get(), 0, nullptr, sImage);
		if (FAILED(hr)) {
			std::string errorStr = name;
			errorStr += "読み込み：失敗";

			MessageBox(NULL, errorStr.c_str(), "MessageBox", MB_OK);
			return false;
		}

		const DirectX::Image* image = sImage.GetImages();

		m_textureData.pData = std::make_unique<uint8_t[]>(sImage.GetPixelsSize());
		std::memcpy(m_textureData.pData.get(), image->pixels, sImage.GetPixelsSize());

		m_textureData.rowPitch		= image->rowPitch;
		m_textureData.slicePitch	= image->slicePitch;
		m_textureData.width			= image->width;
		m_textureData.height		= image->height;
		m_textureData.format		= image->format;

		m_textureName = name;

		return true;
	}

} // namespace