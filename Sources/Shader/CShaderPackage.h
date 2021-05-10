/**
 * @file CShaderPackage.h
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#pragma once

#ifndef _CSHADER_PACKAGE_H_
#define _CSHADER_PACKAGE_H_

#include "CShader.h"

namespace Engine46 {

	// 前方宣言
	class CShaderManager;
	class CDX11Renderer;

	class CShaderPackage {
	private:
		std::vector<std::unique_ptr<CShader>>	m_pVecShader;

		CDX11Renderer*							pDX11Renderer;

		const char*								m_PakageName;

		bool									m_isCompile;
	public:
		CShaderPackage();
		CShaderPackage(CDX11Renderer* pRenderer, const char* name);
		~CShaderPackage();

		bool Initialize(CShaderManager* pShaderManager);

		void SetShader();

		bool CompilePackage(CShaderManager* pShaderManager);

		bool SavePackage(std::ofstream& ofs);
		bool LoadPackage(std::ifstream& ifs);

		void AddShader(std::unique_ptr<CShader>& pShader) { m_pVecShader.emplace_back(move(pShader)); };

		CShader* GetShader(SHADER_TYPE type);

		const char* GetPackageName() const { return m_PakageName; }

		bool GetIsCompile() const { return m_isCompile; }
	};

} // namespace

#endif