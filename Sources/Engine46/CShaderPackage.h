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

	static const std::vector<ShaderInfo> SHADER_INFOS = {
		{ "VS_main", "vs_5_0", SHADER_TYPE::TYPE_VERTEX },
		{ "PS_main", "ps_5_0", SHADER_TYPE::TYPE_PIXEL },
		{ "HS_main", "hs_5_0", SHADER_TYPE::TYPE_HULL },
		{ "DS_main", "ds_5_0", SHADER_TYPE::TYPE_DOMAIN },
		{ "GS_main", "gs_5_0", SHADER_TYPE::TYPE_GEOMETRY },
		{ "CS_main", "cs_5_0", SHADER_TYPE::TYPE_COMPUTE },
	};

	class CShaderPackage {
	protected:
		std::vector<std::unique_ptr<CShaderBase>>	m_pVecShader;

		const char*									m_PakageName;

	public:
		explicit CShaderPackage(const char* name);
		CShaderPackage();
		~CShaderPackage();

		virtual void SetShader() {};

		virtual bool Initialize() { return true; };

		bool CompileShader(ComPtr<ID3DBlob>& pBlob, const char* fileName, const char* entrPoint, const char* shaderModel);

		void AddShaderToVec(std::unique_ptr<CShaderBase>& pShader) { m_pVecShader.emplace_back(move(pShader)); }

		bool SavePackage(std::ofstream& ofs);
		bool LoadPackage(std::ifstream& ifs);

		CShaderBase* GetShader(SHADER_TYPE type);

		const char* GetPackageName() const { return m_PakageName; }

		bool IsCompile() const { return !m_pVecShader.empty(); }
	};

} // namespace

#endif