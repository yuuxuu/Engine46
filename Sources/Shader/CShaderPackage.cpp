/**
 * @file CShaderPackage.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CShaderManager.h"

namespace Engine46 {

	// コンストラクタ
	CShaderPackage::CShaderPackage() :
		m_PakageName(),
		m_isCompile(false)
	{}

	// コンストラクタ
	CShaderPackage::CShaderPackage(const char* name) :
		m_PakageName(name),
		m_isCompile(false)
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

	//	シェーダーパッケージのコンパイル
	bool CShaderPackage::CompilePackage(CShaderManager* pShaderManager) {
		ComPtr<ID3DBlob> pBlob;
		std::unique_ptr<CShader> pShader;

		if (pShaderManager->CompileShader(pBlob, m_PakageName, "VS_main", "vs_5_1")) {
			pShader = std::make_unique<CShader>(m_PakageName, pBlob, SHADER_TYPE::TYPE_VERTEX);

			this->AddShader(pShader);
		}
		if (pShaderManager->CompileShader(pBlob, m_PakageName, "PS_main", "ps_5_1")) {
			pShader = std::make_unique<CShader>(m_PakageName, pBlob, SHADER_TYPE::TYPE_PIXEL);

			this->AddShader(pShader);
		}
		if (pShaderManager->CompileShader(pBlob, m_PakageName, "HS_main", "hs_5_1")) {
			pShader = std::make_unique<CShader>(m_PakageName, pBlob, SHADER_TYPE::TYPE_HULL);

			this->AddShader(pShader);
		}
		if (pShaderManager->CompileShader(pBlob, m_PakageName, "DS_main", "ds_5_1")) {
			pShader = std::make_unique<CShader>(m_PakageName, pBlob, SHADER_TYPE::TYPE_DOMAIN);

			this->AddShader(pShader);
		}
		if (pShaderManager->CompileShader(pBlob, m_PakageName, "GS_main", "gs_5_1")) {
			pShader = std::make_unique<CShader>(m_PakageName, pBlob, SHADER_TYPE::TYPE_GEOMETRY);

			this->AddShader(pShader);
		}
		if (pShaderManager->CompileShader(pBlob, m_PakageName, "CS_main", "cs_5_1")) {
			pShader = std::make_unique<CShader>(m_PakageName, pBlob, SHADER_TYPE::TYPE_COMPUTE);

			this->AddShader(pShader);
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
			std::unique_ptr<CShader> pShader = std::make_unique<CShader>();

			if(pShader->Load(ifs)) {
				this->AddShader(pShader);
			}
		}

		m_isCompile = (m_pVecShader.size() > 0) ? true : false;

		return m_isCompile;
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