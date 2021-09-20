/**
 * @file CShaderPackage.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CShaderPackage.h"
#include "utility.h"

namespace Engine46 {

	// コンストラクタ
	CShaderPackage::CShaderPackage(const char* name) :
		m_PakageName(name)
	{}

	// コンストラクタ
	CShaderPackage::CShaderPackage()
	{}

	// デストラクタ
	CShaderPackage::~CShaderPackage()
	{}

	// シェーダーコンパイル
	bool CShaderPackage::CompileShader(
		ComPtr<ID3DBlob>& pBlob,
		const char* shaderName,
		const char* entryPoint,
		const char* shaderModel) {

		ComPtr<ID3DBlob> pErrBlob;

		DWORD shaderFlags = 0;
#ifdef _DEBUG
		shaderFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		std::unique_ptr<wchar_t[]> name;
		CharConvertToWchar(name, shaderName);

		HRESULT hr = D3DCompileFromFile(
			name.get(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint,
			shaderModel,
			shaderFlags,
			0,
			&pBlob,
			&pErrBlob);

		if (FAILED(hr)) {
			std::cout << name.get() << " " << (char*)pErrBlob->GetBufferPointer() << "コンパイル:失敗" << std::endl;
			return false;
		}

		return true;
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

		return IsCompile();
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