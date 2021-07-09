/**
 * @file CLight.h
 * @brief
 * @author 木村優
 * @date 2021/07/04
 */

#pragma once

#ifndef _CLIGHT_H_
#define _CLIGHT_H_

#include "CActor.h"

namespace Engine46 {

	// 前方宣言
	class CSprite;

	class CLight : public CActorBase {
	private:
		std::unique_ptr<CSprite>				m_pSprite;

		VECTOR4									m_lightDiffuse;
		VECTOR4									m_lightSpecular;
		VECTOR4									m_lightAmbinet;
		VECTOR4									m_lightEmissive;

		VECTOR4									m_lightAttenuation;

	public:
		explicit CLight(const char* lightName);
		~CLight();

		void InitializeResource(CRendererBase* pRenderer) override;

		void Update() override;
		void Draw() override;

		void SetLightDiffuse(const VECTOR4& diffuse) { m_lightDiffuse = diffuse; }
		void SetLightSpecular(const VECTOR4& specular) { m_lightSpecular = specular; }
		void SetLightAmbient(const VECTOR4& ambient) { m_lightAmbinet = ambient; }
		void SetLightEmissive(const VECTOR4& emissive) { m_lightEmissive = emissive; }
		void SetLightAttenuation(const VECTOR4& Attenuation) { m_lightAttenuation = Attenuation; }

		VECTOR4 GetLightDiffuse() const { return m_lightDiffuse; }
		VECTOR4 GetLightSpecular() const { return m_lightSpecular; }
		VECTOR4 GetLightAmbient() const { return m_lightAmbinet; }
		VECTOR4 GetLightEmissive() const { return m_lightEmissive; }
		VECTOR4 GetLightAttenuation() const { return m_lightAttenuation; }
	};

} // namespace

#endif

