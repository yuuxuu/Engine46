/**
 * @file CShader.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CShader.h"
#include "CDataRecord.h"

namespace Engine46 {

	// コンストラクタ
	CShaderBase::CShaderBase() :
		m_shaderName(),
		m_bufSize(0),
		m_shaderType(SHADER_TYPE::TYPE_NONE)
	{}

	// コンストラクタ
	CShaderBase::CShaderBase(const char* name, ComPtr<ID3DBlob>& pBlob, SHADER_TYPE type) :
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
	CShaderBase::~CShaderBase()
	{}

	// 初期化
	void CShaderBase::Initialize() {

		vecDataRecord.clear();

		vecDataRecord.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CShaderBase, m_shaderType), sizeof(m_shaderType)));
		vecDataRecord.emplace_back(std::make_unique<CStrDataRecord>(offsetof(CShaderBase, m_shaderName), m_shaderName));
		vecDataRecord.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CShaderBase, m_bufSize), sizeof(m_bufSize)));
		vecDataRecord.emplace_back(std::make_unique<CBufDataRecord>(m_pBuf, m_bufSize));
	}

	// シェーダーの保存
	bool CShaderBase::Save(std::ofstream& ofs) {
		for (auto& record : vecDataRecord) {
			record->WriteData(ofs, (char*)this);
		}

		return true;
	}

	// シェーダーの読み込み
	bool CShaderBase::Load(std::ifstream& ifs) {
		for (auto& record : vecDataRecord) {
			record->ReadData(ifs, (char*)this);
		}

		return true;
	}

	// データを設定
	void CShaderBase::SetData(ComPtr<ID3DBlob>& pBlob) {
		m_bufSize = pBlob->GetBufferSize();
		m_pBuf.reset(new char[m_bufSize]);

		char* p = (char*)pBlob->GetBufferPointer();
		std::memcpy(m_pBuf.get(), p, m_bufSize);

		m_pBlob = pBlob;
	}

} // namespace