/**
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

	class CDataRecordBase;
	class CDX11Renderer;

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

	class CShader {
	protected:
		std::vector<std::unique_ptr<CDataRecordBase>>	vecDataRecord;

		ComPtr<ID3DBlob>								m_pBlob;

		std::unique_ptr<char[]>							m_shaderName;

		std::unique_ptr<char[]>							m_pBuf;
		int												m_bufSize;

		SHADER_TYPE										m_shaderType;

	public:
		CShader();
		CShader(const char* name, ComPtr<ID3DBlob>& pBlob, SHADER_TYPE type);
		virtual ~CShader();

		void Initialize();

		virtual void Create() {}
		virtual void Set() {}

		bool Save(std::ofstream& ofs);
		bool Load(std::ifstream& ifs);

		void SetData(ComPtr<ID3DBlob>& pBlob);

		SHADER_TYPE GetShaderType() const { return m_shaderType; }
	};

	class CDX11Shader : public CShader {
	private:
		CDX11Renderer*				pDX11Renderer;

		ComPtr<ID3D11VertexShader>	 m_pVS;
		ComPtr<ID3D11PixelShader>	 m_pPS;
		ComPtr<ID3D11HullShader>	 m_pHS;
		ComPtr<ID3D11DomainShader>	 m_pDS;
		ComPtr<ID3D11GeometryShader> m_pGS;
		ComPtr<ID3D11ComputeShader>	 m_pCS;

	public:
		CDX11Shader(CDX11Renderer* pRenderer, const char* name, ComPtr<ID3DBlob>& pBlob, SHADER_TYPE type);
		~CDX11Shader();

		void Create() override;
		void Set() override;
	};

} // namespace

#endif
