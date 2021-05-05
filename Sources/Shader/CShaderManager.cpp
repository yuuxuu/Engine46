﻿/**
 * @file CShaderManager.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CShaderManager.h"
#include "../Engine46/utility.h"

namespace Engine46 {

	constexpr const char* g_ShaderPackageListFileName = "ShaderPackageList.bin";

	// コンストラクタ
	CShaderManager::CShaderManager()
	{}

	// デストラクタ
	CShaderManager::~CShaderManager()
	{}

	// 初期化
	bool CShaderManager::Initialize() {

		this->LoadShaderPackageList();
		
		const char* shaderName = "D:/Engine46/Sources/Shader/ShaderSource/HLSL/Model.hlsl";
		CShaderPackage* pSp = CreateShaderPackage(shaderName);
		
		if (!pSp->GetIsCompile()) {
			if (!pSp->CompilePackage(this)) {
				std::cout << shaderName << "読み込み：失敗" << std::endl;
			}
		}
		
		this->SaveShaderPackageList();

		return true;
	}

	// シェーダーパッケージを作成
	CShaderPackage* CShaderManager::CreateShaderPackage(const char* packageName) {
		auto itr = m_mapShaderPackage.find(packageName);

		if (itr == m_mapShaderPackage.end()) {
			std::unique_ptr<CShaderPackage> sp = std::make_unique<CShaderPackage>(packageName);

			CShaderPackage* pSp = sp.get();

			this->AddShaderPackageToMap(sp->GetPackageName(), sp);

			return pSp;
		}

		return itr->second.get();
	}

	void CShaderManager::AddShaderPackageToMap(const char* name, std::unique_ptr<CShaderPackage>& pSp) {
		auto itr = m_mapShaderPackage.find(name);

		if (itr == m_mapShaderPackage.end()) {
			m_mapShaderPackage[name] = move(pSp);
		}
	}

	// シェーダーの保存
	bool CShaderManager::SaveShaderPackageList() {

		std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary;

		std::ofstream ofs;
		ofs.open(g_ShaderPackageListFileName, mode);

		if (!ofs.is_open()) return false;

		int packageSize = m_mapShaderPackage.size();
		ofs.write((char*)&packageSize, sizeof(int));

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

		int packageSize = m_mapShaderPackage.size();
		ifs.read((char*)&packageSize, sizeof(int));

		for (int i = 0; i < packageSize; ++i) {
			std::unique_ptr<CShaderPackage> sp = std::make_unique<CShaderPackage>();

			if (sp->LoadPackage(ifs)) {
				this->AddShaderPackageToMap(sp->GetPackageName(), sp);
			}
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
			std::cout << name.get() << " " << entryPoint << "コンパイル:失敗" << std::endl;
			return false;
		}

		return true;
	}

} // namespace