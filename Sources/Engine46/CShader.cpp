/**
 * @file CShader.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CShader.h"

namespace Engine46 {

    // コンストラクタ
    CShaderBase::CShaderBase() :
        m_shaderName(),
        m_byteSize(0),
        m_shaderType(SHADER_TYPE::TYPE_NONE)
    {}

    // コンストラクタ
    CShaderBase::CShaderBase(const std::string& name, ComPtr<ID3DBlob>& pBlob, SHADER_TYPE type) :
        m_byteSize(0),
        m_shaderName(name),
        m_shaderType(type)
    {
        m_shaderName.resize(m_shaderName.size());

        this->SetData(pBlob);
    }

    // デストラクタ
    CShaderBase::~CShaderBase()
    {}

    // 初期化
    void CShaderBase::Initialize() 
    {}

    // データを設定
    void CShaderBase::SetData(ComPtr<ID3DBlob>& pBlob) {
        m_byteSize = (int)pBlob->GetBufferSize();
        m_pBuf.reset(new BYTE[m_byteSize]);

        char* p = (char*)pBlob->GetBufferPointer();
        std::memcpy(m_pBuf.get(), p, m_byteSize);

        m_pBlob = pBlob;
    }

} // namespace