/**
 * @file CMaterial.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#include "CMaterial.h"

namespace Engine46 {

	static UINT g_materialCount = 0;

	// コンストラクタ
	CMaterialBase::CMaterialBase() :
		m_diffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_specular(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_ambient(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_emissive(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_brightness(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		pTexture(nullptr),
		pShaderPackage(nullptr),
		m_materialID(g_materialCount++)
	{
		std::string str = "material_" + std::to_string(g_materialCount);
		int size = (int)str.size() + 1;
		m_materialName.reset(new char[size]);
		str.resize(size);
		str.copy(m_materialName.get(), size);
	}

	// コンストラクタ
	CMaterialBase::CMaterialBase(const char* name) :
		m_diffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_specular(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_ambient(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_emissive(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		m_brightness(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)),
		pTexture(nullptr),
		pShaderPackage(nullptr),
		m_materialID(g_materialCount++)
	{
		std::string str = name;
		int size = (int)str.size() + 1;
		m_materialName.reset(new char[size]);
		str.resize(size);
		str.copy(m_materialName.get(), size);
	}

	// デストラクタ
	CMaterialBase::~CMaterialBase()
	{}

} // namespace