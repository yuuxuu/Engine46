﻿/**
 * @file CShader.h
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#pragma once

#ifndef _CSHADER_H_
#define _CSHADER_H_

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	// 前方宣言
	class CDataRecordBase;

	enum class SHADER_TYPE {
		TYPE_VERTEX,				// 頂点
		TYPE_PIXEL,					// ピクセル
		TYPE_HULL,					// ハル
		TYPE_DOMAIN,				// ドメイン
		TYPE_GEOMETRY,				// ジオメトリー
		TYPE_COMPUTE,				// コンピュート
		TYPE_GEOMETRY_STREAM_OUTPUT,// ジオメトリーストリームアウトプット
		TYPE_NONE,
	};

	class CShaderBase {
	protected:
		std::vector<std::unique_ptr<CDataRecordBase>>	vecDataRecord;

		ComPtr<ID3DBlob>								m_pBlob;

		std::string										m_shaderName;

		std::unique_ptr<char[]>							m_pBuf;
		int												m_bufSize;

		SHADER_TYPE										m_shaderType;

	public:
		CShaderBase();
		CShaderBase(const char* name, ComPtr<ID3DBlob>& pBlob, SHADER_TYPE type);
		virtual ~CShaderBase();

		void Initialize();

		virtual void Create() {}
		virtual void Set() {}

		bool Save(std::ofstream& ofs);
		bool Load(std::ifstream& ifs);

		void SetData(ComPtr<ID3DBlob>& pBlob);

		const char* GetShaderName() const { return m_shaderName.c_str(); }

		SHADER_TYPE GetShaderType() const { return m_shaderType; }
	};

} // namespace

#endif