/**
 * @file CTexture.h
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#pragma once

#ifndef _CTEXTURE_H_
#define _CTEXTURE_H_

namespace Engine46 {

	struct TextureData {
		std::unique_ptr<uint8_t[]>	pData;

		size_t						rowPitch;
		size_t						slicePitch;

		UINT						width;
		UINT						height;

		DXGI_FORMAT					format;

		TextureData() :
			pData(nullptr),
			rowPitch(0),
			slicePitch(0),
			width(0),
			height(0),
			format(DXGI_FORMAT_UNKNOWN)
		{}
	};

	class CTextureBase {
	protected:
		const char*		m_textureName;

		TextureData		m_textureData;

	public:
		CTextureBase();
		virtual ~CTextureBase();

		bool Initialize(const char* name);

		bool LoadTexture(const char* name);

		virtual void Create() {};
		virtual void Set(UINT slot) {};

		const char* GetTextureName() const { return m_textureName; }
	};

} // namespace

#endif