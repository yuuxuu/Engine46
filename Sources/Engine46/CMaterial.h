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
#include "CShaderPackage.h"

namespace Engine46 {

	class CMaterialBase {
	protected:
		CTextureBase*				pTexture;

		CShaderPackage*				pShaderPackage;

		VECTOR4						m_diffuse;
		VECTOR4						m_specular;
		VECTOR4						m_ambient;
		VECTOR4						m_emissive;
		VECTOR4						m_brightness;

		int							m_materialID;

		std::unique_ptr<char[]>		m_materialName;

	public:
		CMaterialBase();
		explicit CMaterialBase(const char* name);
		virtual ~CMaterialBase();

		virtual void CreateConstantBuffer() {};
		virtual void Update() {};
		virtual void Set(UINT slot) {};

		void SetDiffuse(const VECTOR4& diffuse) { m_diffuse = diffuse; }
		void SetSpecular(const VECTOR4& specular) { m_diffuse = specular; }
		void SetAmbient(const VECTOR4& ambient) { m_diffuse = ambient; }
		void SetEmissive(const VECTOR4& emissive) { m_diffuse = emissive; }
		void SetBrightness(const VECTOR4& brightness) { m_diffuse = brightness; }

		void SetTexture(CTextureBase* pTexture) { this->pTexture = pTexture; }

		void SetShaderPackage(CShaderPackage* pSp) { pShaderPackage = pSp; }

		char* GetMaterialName() const { return m_materialName.get(); }
	};

} // namespace

#endif