/**
 * @file CShader.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CShader.h"
#include "../Engine46/CDataRecord.h"

#include "../Renderer/CDX11Renderer.h"

namespace Engine46 {

	// コンストラクタ
	CShader::CShader() :
		m_shaderName(),
		m_bufSize(0),
		m_shaderType(SHADER_TYPE::TYPE_NONE)
	{
		std::string str = "shader";
		int size = (int)str.size() + 1;
		m_shaderName.reset(new char[size]);
		str.resize(size);
		str.copy(m_shaderName.get(), size);

		this->Initialize();
	}

	// コンストラクタ
	CShader::CShader(const char* name, ComPtr<ID3DBlob>& pBlob, SHADER_TYPE type) :
		m_bufSize(0),
		m_shaderType(type)
	{
		std::string str = name;
		int size = (int)str.size() + 1;
		m_shaderName.reset(new char[size]);
		str.resize(size);
		str.copy(m_shaderName.get(), size);

		this->SetData(pBlob);

		this->Initialize();
	}

	// デストラクタ
	CShader::~CShader()
	{}

	// 初期化
	void CShader::Initialize() {

		vecDataRecord.clear();

		vecDataRecord.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CShader, m_shaderType), sizeof(m_shaderType)));
		vecDataRecord.emplace_back(std::make_unique<CStrDataRecord>(offsetof(CShader, m_shaderName), m_shaderName));
		vecDataRecord.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CShader, m_bufSize), sizeof(m_bufSize)));
		vecDataRecord.emplace_back(std::make_unique<CBufDataRecord>(m_pBuf, m_bufSize));
	}

	// シェーダーの保存
	bool CShader::Save(std::ofstream& ofs) {
		for (auto& record : vecDataRecord) {
			record->WriteData(ofs, (char*)this);
		}

		return true;
	}

	// シェーダーの読み込み
	bool CShader::Load(std::ifstream& ifs) {
		for (auto& record : vecDataRecord) {
			record->ReadData(ifs, (char*)this);
		}

		return true;
	}

	// データを設定
	void CShader::SetData(ComPtr<ID3DBlob>& pBlob) {
		m_bufSize = pBlob->GetBufferSize();
		m_pBuf.reset(new char[m_bufSize]);

		char* p = (char*)pBlob->GetBufferPointer();
		std::memcpy(m_pBuf.get(), p, m_bufSize);

		m_pBlob = pBlob;
	}

	// コンストラクタ
	CDX11Shader::CDX11Shader(CDX11Renderer* pRenderer, const char* name, ComPtr<ID3DBlob>& pBlob, SHADER_TYPE type) :
		CShader(name, pBlob, type),
		pDX11Renderer(pRenderer)
	{}

	// デストラクタ
	CDX11Shader::~CDX11Shader()
	{}

	// シェーダーを作成
	void CDX11Shader::Create() {
		switch (m_shaderType) {
		case SHADER_TYPE::TYPE_VERTEX:
			pDX11Renderer->CreateVertexShader(m_pVS, m_pBlob.Get());
			break;
		case SHADER_TYPE::TYPE_PIXEL:
			pDX11Renderer->CreatePixelShader(m_pPS, m_pBlob.Get());
			break;
		case SHADER_TYPE::TYPE_HULL:
			pDX11Renderer->CreateHullShader(m_pHS, m_pBlob.Get());
			break;
		case SHADER_TYPE::TYPE_DOMAIN:
			pDX11Renderer->CreateDomainShader(m_pDS, m_pBlob.Get());
			break;
		case SHADER_TYPE::TYPE_GEOMETRY:
			pDX11Renderer->CreateGeometryShader(m_pGS, m_pBlob.Get());
			break;
		case SHADER_TYPE::TYPE_COMPUTE:
			pDX11Renderer->CreateComputeShader(m_pCS, m_pBlob.Get());
			break;
		}
	}

	// シェーダーを設定
	void CDX11Shader::Set() {
		switch (m_shaderType) {
		case SHADER_TYPE::TYPE_VERTEX:
			pDX11Renderer->SetVsShader(m_pVS.Get());
			break;
		case SHADER_TYPE::TYPE_PIXEL:
			pDX11Renderer->SetPsShader(m_pPS.Get());
			break;
		case SHADER_TYPE::TYPE_HULL:
			pDX11Renderer->SetHsShader(m_pHS.Get());
			break;
		case SHADER_TYPE::TYPE_DOMAIN:
			pDX11Renderer->SetDsShader(m_pDS.Get());
			break;
		case SHADER_TYPE::TYPE_GEOMETRY:
			pDX11Renderer->SetGsShader(m_pGS.Get());
			break;
		case SHADER_TYPE::TYPE_COMPUTE:
			pDX11Renderer->SetCsShader(m_pCS.Get());
			break;
		}
	}

} // namespace