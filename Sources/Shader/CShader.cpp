/**
 * @file CShader.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CShader.h"
#include "../Engine46/CDataRecord.h"

namespace Engine46 {

	// コンストラクタ
	CShader::CShader() :
		m_bufSize(0),
		m_shaderType(SHADER_TYPE::TYPE_NONE)
	{}

	// コンストラクタ
	CShader::CShader(const char* name, SHADER_TYPE type) :
		m_shaderType(type)
	{
		std::string str = name;
		int size = (int)str.size() + 1;
		m_shaderName.reset(new char[size]);
		str.resize(size);
		str.copy(m_shaderName.get(), size);
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
		vecDataRecord.emplace_back(std::make_unique<CStrDataRecord>(offsetof(CShader, m_pBuf), m_pBuf));
	}

	// シェーダーの保存
	bool CShader::Save(std::ofstream& ofs) {
		for (auto& record : vecDataRecord) {
			record->WriteData(ofs, (char*)this);
		}

		//CFileSystem fs;
		//std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary;
		//
		//if (!fs.WriteFile(m_shaderName.get(), mode, m_pBuf.get(), m_bufSize)) return false;
		
		return true;
	}

	// シェーダーの読み込み
	bool CShader::Load(std::ifstream& ifs) {
		for (auto& record : vecDataRecord) {
			record->ReadData(ifs, (char*)this);
		}

		//CFileSystem fs;
		//std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary;
		//
		//if (!fs.ReadFile(m_shaderName.get(), mode, (void*&)m_pBuf, m_bufSize)) return false;

		return true;
	}

} // namespace