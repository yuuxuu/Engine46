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
        std::unique_ptr<uint8_t[]>  pData;

        UINT                        rowPitch;
        UINT                        slicePitch;

        UINT                        width;
        UINT                        height;

        DXGI_FORMAT                 format;

        TextureData() :
            pData(nullptr),
            rowPitch(0),
            slicePitch(0),
            width(0),
            height(0),
            format(DXGI_FORMAT_UNKNOWN)
        {}
    };

    enum class TextureType {
        Render,
        Depth,
        Stencil,
        Image,
    };

    class CTextureBase {
    protected:
        std::string	m_textureName;

        TextureData	m_textureData;

    public:
        CTextureBase();
        CTextureBase(const char* textureName);
        virtual ~CTextureBase();

        bool Initialize();

        bool LoadTexture(const char* filePath);

        virtual void CreateTexture() {};
        virtual void CreateShaderResourceView() {};
        virtual void Set(UINT slot) {};
        virtual void SetCompute(UINT slot) {};

        std::string GetTextureName() const { return m_textureName.c_str(); }

        UINT GetTextureWidth() const { return m_textureData.width; }
        UINT GetTextureHeight() const { return m_textureData.height; }
    };

} // namespace

#endif