/**
 * @file CShaderPackage.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CShaderManager.h"

#include "../GraphicsAPI/CDX11Renderer.h"
#include "../GraphicsAPI/CDX11Shader.h"

namespace Engine46 {

	// コンストラクタ
	CShaderPackage::CShaderPackage() :
		m_PakageName(),
		m_layoutBufSize(0),
		m_isCompile(false)
	{}

	// コンストラクタ
	CShaderPackage::CShaderPackage(CDX11Renderer* pRenderer, const char* name) :
		pDX11Renderer(pRenderer),
		m_layoutBufSize(0),
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

	// シェーダーパッケージのシェーダーをそれぞれ設定
	void CShaderPackage::SetShader() {
		CDX11DeviceContext* pDeviceContext = pDX11Renderer->GetDX11DeviceContext();
		pDeviceContext->SetInputLayout(m_pInputLayout.Get());

		for (const auto& shader : m_pVecShader) {
			shader->Set();
		}
	}

	//	シェーダーパッケージのコンパイル
	bool CShaderPackage::CompilePackage(CShaderManager* pShaderManager) {
		
		struct shaderInfo {
			const char* entryPoint;
			const char* shaderModel;
			SHADER_TYPE shadeType;
		};
		static std::vector<shaderInfo> vecShaderInfo = {
			{ "VS_main", "vs_5_0", SHADER_TYPE::TYPE_VERTEX },
			{ "PS_main", "ps_5_0", SHADER_TYPE::TYPE_PIXEL },
			{ "HS_main", "hs_5_0", SHADER_TYPE::TYPE_HULL },
			{ "DS_main", "ds_5_0", SHADER_TYPE::TYPE_DOMAIN },
			{ "GS_main", "gs_5_0", SHADER_TYPE::TYPE_GEOMETRY },
			{ "CS_main", "cs_5_0", SHADER_TYPE::TYPE_COMPUTE },
		};

		ComPtr<ID3DBlob> pBlob;
		std::unique_ptr<CShader> pShader;

		if (pDX11Renderer) {
			CDX11Device* pDevice = pDX11Renderer->GetDX11Device();
			CDX11DeviceContext* pDeviceContext = pDX11Renderer->GetDX11DeviceContext();

			for (const auto& info : vecShaderInfo) {
				if (pShaderManager->CompileShader(pBlob, m_PakageName, info.entryPoint, info.shaderModel)) {
					if (m_layoutBufSize < pBlob->GetBufferSize()) {
						m_layoutBufSize = pBlob->GetBufferSize();
						pDevice->CreateInputLayout(m_pInputLayout, pBlob->GetBufferPointer(), pBlob->GetBufferSize());
					}
					
					pShader = std::make_unique<CDX11Shader>(pDevice, pDeviceContext, m_PakageName, pBlob, info.shadeType);
					
					pShader->Create();

					this->AddShader(pShader);
				}
			}
		}
		else {
			for (const auto& info : vecShaderInfo) {
				if (pShaderManager->CompileShader(pBlob, m_PakageName, info.entryPoint, info.shaderModel)) {
					pShader = std::make_unique<CShader>(m_PakageName, pBlob, info.shadeType);

					this->AddShader(pShader);
				}
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