/**
 * @file CShaderManager.h
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#pragma once

#ifndef _CSHADER_MANAGER_H_
#define _CSHADER_MANAGER_H_

#include "CShaderPackage.h"

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	// 前方宣言
	class CRendererBase;
	class CShaderPackage;

	class CShaderManager {
	private:
		std::map<const char*, std::unique_ptr<CShaderPackage>>	m_pMapShaderPackage;

		CRendererBase*											pRenderer;

	public:
		explicit CShaderManager(CRendererBase* pRenderer);
		~CShaderManager();

		bool Initialize();

		bool SaveShaderPackageList();
		bool LoadShaderPackageList();

		bool CompileShader(ComPtr<ID3DBlob>& pBlob, const char* fileName, const char* entrPoint, const char* shaderModel);

		CShaderPackage* GetShaderPackage(const char* name);
		
		CShaderPackage* CreateShaderPackage(const char* packageName);

		void AddShaderPackageToMap(const char* name, std::unique_ptr<CShaderPackage>& pSp);
	};

} // namespace

#endif
