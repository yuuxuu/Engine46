/**
 * @file CMaterial.h
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#pragma once

#ifndef _CMATERIAL_H_
#define _CMATERIAL_H_

#include "CTexture.h"

namespace Engine46 {

	// 前方宣言
	class CDX11Renderer;
	class CDX11CB;

	class CMaterialBase {
	protected:
		CTextureBase*	pTexture;

		VECTOR4			m_diffuse;
		VECTOR4			m_specular;
		VECTOR4			m_ambient;
		VECTOR4			m_emissive;
		VECTOR4			m_brightness;

	public:
		CMaterialBase();
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
	};

	class CDX11Material : public CMaterialBase {
	private:
		CDX11Renderer*			pDX11Renderer;

		std::unique_ptr<CDX11CB> m_pConstantBuffer;

	public:
		CDX11Material(CDX11Renderer* pRenderer);
		~CDX11Material();

		void CreateConstantBuffer() override;
		void Update() override;
		void Set(UINT slot) override;
	};

} // namespace

#endif