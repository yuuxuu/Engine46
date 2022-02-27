/**
 * @file CTexture.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#include "CTexture.h"
#include "utility.h"
#include "math.h"

#include "CGameSystem.h"
#include "CFileManager.h"

namespace Engine46 {

    // コンストラクタ
    CTextureBase::CTextureBase() :
        m_textureData(),
        m_textureName()
    {}

    // コンストラクタ
    CTextureBase::CTextureBase(const std::string& textureName) :
        m_textureData(),
        m_textureName(textureName)
    {}

    // デストラクタ
    CTextureBase::~CTextureBase()
    {}

    // 初期化
    bool CTextureBase::Initialize() {

        return true;
    }

    // テクスチャを読み込む
    bool CTextureBase::LoadTexture(const std::string& textureName) {
        if (m_isLoaded) return true;
        
        FileInfo* pFileInfo = CFileManager::GetFileManager().GetFileInfoFromMap(textureName);
        if (!pFileInfo) return false;
        
        std::unique_ptr<wchar_t[]> loadName;
        CharConvertToWchar(loadName, pFileInfo->filePath.c_str());

        DirectX::ScratchImage sImage;
        HRESULT hr = DirectX::LoadFromWICFile(loadName.get(), 0, nullptr, sImage);
        if (FAILED(hr)) {
            std::string errorStr = pFileInfo->filePath;
            errorStr += "読み込み：失敗";

            MessageBox(NULL, errorStr.c_str(), "MessageBox", MB_OK);
            return false;
        }

        const DirectX::Image* image = sImage.GetImages();

        m_textureData.pixelsSize = (UINT)sImage.GetPixelsSize();
        m_textureData.pData = std::make_unique<uint8_t[]>(m_textureData.pixelsSize);
        std::memcpy(m_textureData.pData.get(), image->pixels, m_textureData.pixelsSize);

        m_textureData.rowPitch = (UINT)image->rowPitch;
        m_textureData.slicePitch = (UINT)image->slicePitch;
        m_textureData.width = (UINT)image->width;
        m_textureData.height = (UINT)image->height;
        m_textureData.format = image->format;

        m_isLoaded = true;

        return true;
    }

    // テクスチャをキューブマップテクスチャに変換
    void CTextureBase::TextureConvertToCubeMapTexture() {

        if (!m_textureData.pData) return;

        const UINT cubeSize = 512;
        const UINT pixelSize = 4;

        m_cubeTextureData.width = cubeSize;
        m_cubeTextureData.height = cubeSize;
        m_cubeTextureData.rowPitch = cubeSize * pixelSize;
        m_cubeTextureData.slicePitch = m_cubeTextureData.rowPitch * cubeSize;
        m_cubeTextureData.pixelsSize = m_cubeTextureData.slicePitch;
        m_cubeTextureData.format = m_textureData.format;

        for (int i = 0; i < CUBE_FACE_MAX; ++i) {
            m_cubeTextureData.pDatas[i] = std::make_unique<uint8_t[]>(m_cubeTextureData.pixelsSize);

            for (UINT h = 0; h < m_cubeTextureData.height; ++h) {
                for (UINT w = 0; w < m_cubeTextureData.width; ++w) {
                    VECTOR2 uv = { (float)w / m_cubeTextureData.width, (float)h / m_cubeTextureData.height };

                    VECTOR3 faceVector;
                    switch (i) {
                    case 0:
                        faceVector = { 
                            1.0f,
                            2.0f * -uv.y + 1.0f,
                            2.0f * -uv.x + 1.0f
                        };
                        break;
                    case 1:
                        faceVector = { 
                            -1.0f,
                            2.0f * -uv.y + 1.0f,
                            2.0f *  uv.x - 1.0f
                        };
                        break;
                    case 2:
                        faceVector = {
                            2.0f * uv.x - 1.0f,
                            1.0f,
                            2.0f *  uv.y - 1.0f
                        };
                        break;
                    case 3:
                        faceVector = {
                            2.0f * uv.x - 1.0f,
                            -1.0f,
                            2.0f * -uv.y + 1.0f
                        };
                        break;
                    case 4:
                        faceVector = {
                            2.0f * uv.x - 1.0f,
                            2.0f * -uv.y + 1.0f,
                            1.0f
                        };
                        break;
                    case 5:
                        faceVector = {
                            2.0f * -uv.x + 1.0f,
                            2.0f * -uv.y + 1.0f,
                            -1.0f
                        };
                        break;
                    }

                    float len = Vec3Lenght(faceVector);

                    float theta = atan2f(faceVector.z, faceVector.x);
                    float phi = std::acosf(faceVector.y / len);

                    uv.x = theta / (2.0f * PI);
                    if (uv.x < 0) uv.x += 1.0f;
                    uv.y = phi / PI;

                    UINT imageXPixel = UINT(m_textureData.width * uv.x);
                    UINT imageYPixel = UINT(m_textureData.height * uv.y);

                    uint8_t r = m_textureData.pData[imageXPixel * pixelSize + imageYPixel * m_textureData.rowPitch];
                    uint8_t g = m_textureData.pData[imageXPixel * pixelSize + imageYPixel * m_textureData.rowPitch + 1];
                    uint8_t b = m_textureData.pData[imageXPixel * pixelSize + imageYPixel * m_textureData.rowPitch + 2];

                    m_cubeTextureData.pDatas[i][w * pixelSize + h * m_cubeTextureData.rowPitch] = r;
                    m_cubeTextureData.pDatas[i][w * pixelSize + h * m_cubeTextureData.rowPitch + 1] = g;
                    m_cubeTextureData.pDatas[i][w * pixelSize + h * m_cubeTextureData.rowPitch + 2] = b;
                    m_cubeTextureData.pDatas[i][w * pixelSize + h * m_cubeTextureData.rowPitch + 3] = 0xff;
                }
            }
        }
    }

} // namespace