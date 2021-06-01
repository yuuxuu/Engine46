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

	struct ShaderInfo {
		const char* entryPoint;
		const char* shaderModel;
		SHADER_TYPE shadeType;
	};

	static const std::vector<ShaderInfo> vecShaderInfo = {
		{ "VS_main", "vs_5_0", SHADER_TYPE::TYPE_VERTEX },
		{ "PS_main", "ps_5_0", SHADER_TYPE::TYPE_PIXEL },
		{ "HS_main", "hs_5_0", SHADER_TYPE::TYPE_HULL },
		{ "DS_main", "ds_5_0", SHADER_TYPE::TYPE_DOMAIN },
		{ "GS_main", "gs_5_0", SHADER_TYPE::TYPE_GEOMETRY },
		{ "CS_main", "cs_5_0", SHADER_TYPE::TYPE_COMPUTE },
	};

	class CShaderPackage {
	private:
		std::vector<std::unique_ptr<CShaderBase>>	m_pVecShader;

		const char*									m_PakageName;
	public:
		explicit CShaderPackage(const char* name);
		CShaderPackage();
		~CShaderPackage();

		void SetShader();

		bool CompileShader(ComPtr<ID3DBlob>& pBlob, const char* fileName, const char* entrPoint, const char* shaderModel);

		bool SavePackage(std::ofstream& ofs);
		bool LoadPackage(std::ifstream& ifs);

		void AddShaderToVec(std::unique_ptr<CShaderBase>& pShader);

		CShaderBase* GetShader(SHADER_TYPE type);

		const char* GetPackageName() const { return m_PakageName; }

		bool IsCompile() const { return !m_pVecShader.empty(); }
	};

} // namespace

#endif