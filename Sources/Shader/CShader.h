/**
 * @file CShader.h
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#pragma once

#ifndef _CSHADER_H_
#define _CSHADER_H_

namespace Engine46 {

	class CDataRecordBase;

	enum class SHADER_TYPE {
		TYPE_VERTEX,				// 頂点
		TYPE_PIXEL,					// ピクセル
		TYPE_HULL,					// ハル
		TYPE_DOMAIN,				// ドメイン
		TYPE_GEOMETRY,				// ジオメトリー
		TYPE_GEOMETRY_STREAM_OUTPUT,// ジオメトリーストリームアウトプット
		TYPE_COMPUTE,				// コンピュート
		TYPE_NONE,
	};

	class CShader {
	protected:
		std::vector<std::unique_ptr<CDataRecordBase>>	vecDataRecord;

		std::unique_ptr<char[]>							m_shaderName;

		std::unique_ptr<char[]>							m_pBuf;
		size_t											m_bufSize;

		SHADER_TYPE										m_shaderType;

	public:
		CShader();
		CShader(const char* name, SHADER_TYPE type);
		~CShader();

		void Initialize();

		bool Save(std::ofstream& ofs);
		bool Load(std::ifstream& ifs);

		SHADER_TYPE GetShaderType() const { return m_shaderType; }
	};

} // namespace

#endif
