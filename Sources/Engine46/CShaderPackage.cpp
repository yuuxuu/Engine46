/**
 * @file CShaderPackage.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CShaderPackage.h"
#include "utility.h"

namespace Engine46 {

    // コンストラクタ
    CShaderPackage::CShaderPackage(const std::string& name) :
        m_PakageName(name)
    {}

    // コンストラクタ
    CShaderPackage::CShaderPackage()
    {}

    // デストラクタ
    CShaderPackage::~CShaderPackage()
    {}

    // シェーダーコンパイル
    bool CShaderPackage::CompileShader(
        ComPtr<ID3DBlob>& pBlob,
        const char* shaderName,
        const char* entryPoint,
        const char* shaderModel) {

        ComPtr<ID3DBlob> pErrBlob;

        DWORD shaderFlags = 0;
#ifdef _DEBUG
        shaderFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        std::unique_ptr<wchar_t[]> name;
        CharConvertToWchar(name, shaderName);

        HRESULT hr = D3DCompileFromFile(
            name.get(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint,
            shaderModel,
            shaderFlags,
            0,
            &pBlob,
            &pErrBlob);

        if (FAILED(hr)) {
            std::cout << name.get() << " " << (char*)pErrBlob->GetBufferPointer() << "コンパイル:失敗" << std::endl;
            return false;
        }

        return true;
    }

    // シェーダーの取得
    CShaderBase* CShaderPackage::GetShader(SHADER_TYPE type) {
        for (const auto& shader : m_pVecShader) {
            if (shader->GetShaderType() == type) {
                return shader.get();
            }
        }

        return nullptr;
    }

} // namespace