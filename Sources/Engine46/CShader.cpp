﻿/**
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
        m_byteSize(0),
        m_shaderType(SHADER_TYPE::TYPE_NONE)
    {}

    // コンストラクタ
    CShaderBase::CShaderBase(const char* name, ComPtr<ID3DBlob>& pBlob, SHADER_TYPE type) :
        m_byteSize(0),
        m_shaderName(name),
        m_shaderType(type)
    {
        m_shaderName.resize(m_shaderName.size());

        this->SetData(pBlob);

        this->Initialize();
    }

    // デストラクタ
    CShaderBase::~CShaderBase()
    {}

    // 初期化
    void CShaderBase::Initialize() {

        vecDataRecord.clear();

        vecDataRecord.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CShaderBase, m_shaderType), sizeof(UINT)));
        vecDataRecord.emplace_back(std::make_unique<CStrDataRecord>(offsetof(CShaderBase, m_shaderName), m_shaderName));
        vecDataRecord.emplace_back(std::make_unique<CDataRecordBase>(offsetof(CShaderBase, m_byteSize), sizeof(m_byteSize)));
        vecDataRecord.emplace_back(std::make_unique<CBufDataRecord>(m_pBuf, m_byteSize));
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
        m_byteSize = (int)pBlob->GetBufferSize();
        m_pBuf.reset(new BYTE[m_byteSize]);

        char* p = (char*)pBlob->GetBufferPointer();
        std::memcpy(m_pBuf.get(), p, m_byteSize);

        m_pBlob = pBlob;
    }

} // namespace