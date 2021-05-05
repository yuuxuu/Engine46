/**
 * @file CShaderPackage.h
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#pragma once

#include "CShader.h"

namespace Engine46 {

	class CShaderManager;

	class CShaderPackage {
	private:
		std::vector<std::unique_ptr<CShader>>	m_pVecShader;

		const char*								m_PakageName;

		bool									m_isCompile;
	public:
		CShaderPackage();
		CShaderPackage(const char* name);
		~CShaderPackage();

		bool CompilePackage(CShaderManager* pSm);

		bool SavePackage(std::ofstream& ofs);
		bool LoadPackage(std::ifstream& ifs);

		void ReleasePackage();

		CShader* GetShader(SHADER_TYPE type);

		void AddShader(std::unique_ptr<CShader>& pShader) { m_pVecShader.emplace_back(move(pShader)); };

		const char* GetPackageName() const { return m_PakageName; }

		bool GetIsCompile() const { return m_isCompile; }
	};

} // namespace
