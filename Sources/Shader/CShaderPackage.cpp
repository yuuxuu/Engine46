/**
 * @file CShaderPackage.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CShaderPackage.h"

namespace Engine46 {

	// コンストラクタ
	CShaderPackage::CShaderPackage()
	{}

	// デストラクタ
	CShaderPackage::~CShaderPackage()
	{}
	
	// シェーダーパッケージを保存
	bool CShaderPackage::SavePackage(std::ofstream& ofs) {
		for (const auto& shader : m_pVecShader) {
			shader->Save(ofs);
		}

		return true;
	}

	// シェーダーパッケージを読み込み
	bool CShaderPackage::LoadPackage(std::ifstream& ifs) {
		for (const auto& shader : m_pVecShader) {
			shader->Load(ifs);
		}

		return true;
	}

	// シェーダーパッケージの解放
	void CShaderPackage::ReleasePackage() {
		for (auto& shader : m_pVecShader) {
			shader.release();
		}

		m_pVecShader.clear();
		m_pVecShader.shrink_to_fit();
	}

	// シェーダーの取得
	CShader* CShaderPackage::GetShader(SHADER_TYPE type) {
		for (const auto& shader : m_pVecShader) {
			if (shader->GetShaderType() == type) {
				return shader.get();
			}
		}

		return nullptr;
	}

} // namespace