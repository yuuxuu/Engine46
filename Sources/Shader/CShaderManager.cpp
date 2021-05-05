/**
 * @file CShaderManager.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CShaderManager.h"
#include "../Engine46/utility.h"

namespace Engine46 {

	constexpr const char* g_ShaderPackageListFileName = "ShaderSources/Binary/ShaderPackageList.bin";

	// コンストラクタ
	CShaderManager::CShaderManager()
	{}

	// デストラクタ
	CShaderManager::~CShaderManager()
	{}

	// 初期化
	bool CShaderManager::Initialize() {



		return true;
	}

	// シェーダーパッケージを作成
	CShaderPackage* CShaderManager::CreateShaderPackage() {
		std::unique_ptr<CShaderPackage> sp = std::make_unique<CShaderPackage>();

		CShaderPackage* pSp = sp.get();

		this->AddShaderPackageToMap(sp->GetPackageName(), sp);

		return pSp;
	}

	// シェーダーの保存
	bool CShaderManager::SaveShaderPackageList() {

		std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary;

		std::ofstream ofs;
		ofs.open(g_ShaderPackageListFileName, mode);

		if (!ofs.is_open()) return false;

		for (const auto& pSp : m_mapShaderPackage) {
			if (!pSp.second->SavePackage(ofs)) continue;
		}

		return true;
	}

	// シェーダーの読み込み
	bool CShaderManager::LoadShaderPackageList() {

		std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary;

		std::ifstream ifs;
		ifs.open(g_ShaderPackageListFileName, mode);

		if (!ifs.is_open()) return false;

		while (true) {

			if (ifs.eof()) break;

			CShaderPackage* pSp = CreateShaderPackage();

			pSp->LoadPackage(ifs);
		}

		return true;
	}

	// シェーダーコンパイル
	bool CShaderManager::CompileShader(
		ComPtr<ID3DBlob>& pBlob,
		const char* shaderName,
		const char* entryPoint,
		const char* shaderModel) {

		ComPtr<ID3DBlob> pErrBlob = nullptr;
		DWORD shaderFlags = 0;
		wchar_t* name = CharConvertToWchar(shaderName);

		HRESULT hr = D3DCompileFromFile(
			name,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint,
			shaderModel,
			shaderFlags,
			0,
			&pBlob,
			&pErrBlob);

		if (FAILED(hr)) {
			std::cout << name << " " << entryPoint << "コンパイル:失敗" << std::endl;
			return false;
		}

		return true;
	}

} // namespace