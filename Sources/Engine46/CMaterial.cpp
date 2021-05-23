/**
 * @file CMaterial.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#include "CMaterial.h"

namespace Engine46 {

	// コンストラクタ
	CMaterialBase::CMaterialBase() :
		m_diffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_specular(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_ambient(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_emissive(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_brightness(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		pTexture(nullptr),
		pShaderPackage(nullptr)
	{}

	// デストラクタ
	CMaterialBase::~CMaterialBase()
	{}

} // namespace