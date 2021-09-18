/**
 * @file CMaterial.h
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#pragma once

#ifndef _CMATERIAL_H_
#define _CMATERIAL_H_

#include "math.h"
#include "CTexture.h"
#include "CConstantBuffer.h"

namespace Engine46 {

	class CMaterialBase {
	protected:
		CTextureBase*							pTexture;
		std::vector<CTextureBase*>				pVecTexture;

		std::unique_ptr<CConstantBufferBase>	m_pMaterialConstantBuffer;

		VECTOR4									m_diffuse;
		VECTOR4									m_specular;
		VECTOR4									m_ambient;
		VECTOR4									m_emissive;

		int										m_materialID;

		std::string								m_materialName;

		bool									m_isInitialize;

	public:
		CMaterialBase();
		explicit CMaterialBase(const char* materialName);
		virtual ~CMaterialBase();

		virtual void Update();
		virtual void Set(UINT slot);

		void SetMaterialConstantBuffer(std::unique_ptr<CConstantBufferBase>& pConstantBuffer);

		void SetDiffuse(const VECTOR4& diffuse) { m_diffuse = diffuse; }
		void SetSpecular(const VECTOR4& specular) { m_diffuse = specular; }
		void SetAmbient(const VECTOR4& ambient) { m_diffuse = ambient; }
		void SetEmissive(const VECTOR4& emissive) { m_diffuse = emissive; }

		CConstantBufferBase* GetMaterialConstantBuffer() const { return m_pMaterialConstantBuffer.get(); }

		void SetTexture(CTextureBase* pTexture) { this->pTexture = pTexture; }

		std::string GetMaterialName() const { return m_materialName.c_str(); }

		bool IsInitialize() const { return m_isInitialize; }
	};

} // namespace

#endif