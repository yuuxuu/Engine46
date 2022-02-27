/**
 * @file CShaderManager.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CShaderManager.h"
#include "CShaderPackage.h"
#include "CActor.h"

#include "CRenderer.h"

namespace Engine46 {

    // コンストラクタ
    CShaderManager::CShaderManager(CRendererBase* pRenderer) :
        pRenderer(pRenderer)
    {}

    // デストラクタ
    CShaderManager::~CShaderManager()
    {}

    // シェーダーパッケージを作成
    CShaderPackage* CShaderManager::CreateShaderPackage(const std::string& packageName) {
        CShaderPackage* pSp = GetShaderPackageFromMap(packageName);

        if (pSp) return pSp;

        std::unique_ptr<CShaderPackage> sp;
        pRenderer->CreateShader(sp, packageName);

        if (sp) {
            pSp = sp.get();

            AddShaderPackageToMap(packageName, sp);

            return pSp;
        }

        return nullptr;
    }

    // シェーダーパッケージをマップへ追加
    void CShaderManager::AddShaderPackageToMap(const std::string& name, std::unique_ptr<CShaderPackage>& pSp) {

        if (!GetShaderPackageFromMap(name)) {
            m_pMapShaderPackage[name] = std::move(pSp);
        }
    }

    // シェーダーパッケージを取得
    CShaderPackage* CShaderManager::GetShaderPackageFromMap(const std::string& name) {
        auto itr = m_pMapShaderPackage.find(name);

        if (itr != m_pMapShaderPackage.end()) {
            return itr->second.get();
        }

        return nullptr;
    }

} // namespace