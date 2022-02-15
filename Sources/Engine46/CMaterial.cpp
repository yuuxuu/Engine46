﻿/**
 * @file CMaterial.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#include "CMaterial.h"

#include "CGameSystem.h"
#include "CTextureManager.h"

#include "GraphicsAPI/DirectX12/CDX12ShaderPackage.h"

namespace Engine46 {

    // コンストラクタ
    CMaterialBase::CMaterialBase() :
        m_diffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_specular(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_ambient(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_emissive(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        pTexture(nullptr),
        m_materialID(0),
        m_materialName("Material_" + std::to_string(m_materialID)),
        m_isInitialize(false)
    {}

    // コンストラクタ
    CMaterialBase::CMaterialBase(const std::string& materialName) :
        m_diffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_specular(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_ambient(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        m_emissive(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
        pTexture(nullptr),
        m_materialID(0),
        m_materialName(materialName),
        m_isInitialize(false)
    {}

    // デストラクタ
    CMaterialBase::~CMaterialBase()
    {}

    // 更新
    void CMaterialBase::Update() {
        if (m_pMaterialConstantBuffer) {
            materialCB cb = {
                m_diffuse,
                m_specular,
                m_ambient,
                m_emissive,
            };

            m_pMaterialConstantBuffer->Update(&cb);
        }
    }

    // マテリアルをシェーダーへ設定
    void CMaterialBase::Set(UINT slot) {
        if (m_pMaterialConstantBuffer) {
            m_pMaterialConstantBuffer->Set(slot);
        }
    }

    // テクスチャをシェーダーへ設定
    void CMaterialBase::SetTexture(UINT slot) {
        if (pTexture) {
            pTexture->Set(slot);
        }
        else {
            for (const auto pTexture : m_pVecTexture) {
                pTexture->Set(slot);
            }
        }
    }

    // コンスタントバッファを設定
    void CMaterialBase::SetMaterialConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer) {

        if (m_isInitialize) return;

        if (pConstantBuffer) {
            m_pMaterialConstantBuffer.swap(pConstantBuffer);

            this->Update();

            m_isInitialize = true;
        }
    }

    // テクスチャを設定
    void CMaterialBase::SetTexture(const std::string& textureName) {
        CTextureManager* textureManager = CGameSystem::GetGameSystem().GetTextureManager();
        CTextureBase* pTexture = textureManager->CreateTexture(textureName);
        if (pTexture) {
            this->pTexture = pTexture;
        }
    }

} // namespace