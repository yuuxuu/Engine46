/**
 * @file CShaderPackage.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CShaderManager.h"

namespace Engine46 {

	// コンストラクタ
	CShaderPackage::CShaderPackage(const char* name) :
		m_PakageName(name),
		m_isCompile(false)
	{}

	// コンストラクタ
	CShaderPackage::CShaderPackage()
	{}

	// デストラクタ
	CShaderPackage::~CShaderPackage()
	{}

	// 初期化
	bool CShaderPackage::Initialize(CShaderManager* pShaderManager) {
		
		if (m_isCompile) return true;

		if (!this->CompilePackage(pShaderManager)) {
			std::cout << m_PakageName << "読み込み：失敗" << std::endl;

			return false;
		}

		return true;
	}

	// シェーダーパッケージのシェーダーをそれぞれ設定
	void CShaderPackage::SetShader() {
		for (const auto& shader : m_pVecShader) {
			shader->Set();
		}
	}

	//	シェーダーパッケージのコンパイル
	bool CShaderPackage::CompilePackage(CShaderManager* pShaderManager) {

		for (const auto& info : vecShaderInfo) {
			ComPtr<ID3DBlob> pBlob;

			if (pShaderManager->CompileShader(pBlob, m_PakageName, info.entryPoint, info.shaderModel)) {
				std::unique_ptr<CShaderBase> pShader = std::make_unique<CShaderBase>(m_PakageName, pBlob, info.shadeType);

				this->AddShaderToVec(pShader);
			}
		}

		m_isCompile = (m_pVecShader.size() > 0) ? true : false;

		return m_isCompile;
	}
	
	// シェーダーパッケージを保存
	bool CShaderPackage::SavePackage(std::ofstream& ofs) {
		int strSize = (int)strlen(m_PakageName) + 1;
		ofs.write((char*)&strSize, sizeof(int));
		ofs.write((char*)&m_PakageName, strSize);

		int packageSize = m_pVecShader.size();
		ofs.write((char*)&packageSize, sizeof(int));

		for (const auto& shader : m_pVecShader) {
			if(!shader->Save(ofs)) continue;
		}

		return true;
	}

	// シェーダーパッケージを読み込み
	bool CShaderPackage::LoadPackage(std::ifstream& ifs) {
		int strSize = 0;
		ifs.read((char*)&strSize, sizeof(int));
		ifs.read((char*)&m_PakageName, strSize);

		int packageSize = 0;
		ifs.read((char*)&packageSize, sizeof(int));
		
		for (int i = 0; i < packageSize;++i) {
			std::unique_ptr<CShaderBase> pShader = std::make_unique<CShaderBase>();

			if(pShader->Load(ifs)) {
				this->AddShaderToVec(pShader);
			}
		}

		m_isCompile = (m_pVecShader.size() > 0) ? true : false;

		return m_isCompile;
	}

	// シェーダーを追加
	void CShaderPackage::AddShaderToVec(std::unique_ptr<CShaderBase>& pShader) {
		m_pVecShader.emplace_back(move(pShader));

		if (!m_isCompile) {
			m_isCompile = !m_pVecShader.empty();
		}
	}

	// シェーダーの取得
	CShaderBase* CShaderPackage::GetShader(SHADER_TYPE type) {
		for (const auto& shader : m_pVecShader) {
			if (shader->GetShaderType() == type) {
				return shader.get();
			}
		}

		return nullptr;
	}

} // namespace