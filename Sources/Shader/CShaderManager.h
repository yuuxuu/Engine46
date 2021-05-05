/**
 * @file CShaderManager.h
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#pragma once

#include "CShaderPackage.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	class CShaderManager {
	private:
		std::map<const char*, std::unique_ptr<CShaderPackage>> m_mapShaderPackage;

	public:
		CShaderManager();
		~CShaderManager();

		bool Initialize();

		CShaderPackage* CreateShaderPackage();

		bool SaveShaderPackageList();
		bool LoadShaderPackageList();

		bool CompileShader(ComPtr<ID3DBlob>& pBlob, const char* fileName, const char* entrPoint, const char* shaderModel);

		void AddShaderPackageToMap(const char* name, std::unique_ptr<CShaderPackage>& pSP) { m_mapShaderPackage[name] = move(pSP); }
	};

} // namespace
