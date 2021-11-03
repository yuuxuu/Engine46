/**
 * @file CShaderManager.cpp
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#include "CShaderManager.h"
#include "CShaderPackage.h"
#include "CActor.h"

#include "GraphicsAPI/CRenderer.h"

namespace Engine46 {

    constexpr const char* g_ShaderPackageListFileName = "ShaderPackageList.bin";

    // コンストラクタ
    CShaderManager::CShaderManager(CRendererBase* pRenderer) :
        pRenderer(pRenderer)
    {}

    // デストラクタ
    CShaderManager::~CShaderManager()
    {}

    // シェーダーパッケージを作成
    CShaderPackage* CShaderManager::CreateShaderPackage(const char* packageName) {
        CShaderPackage* pSp = GetShaderPackageFromMap(packageName);

        if (pSp) return pSp;

        std::unique_ptr<CShaderPackage> sp;
        pRenderer->CreateShader(sp, packageName);

        if (sp) {
            pSp = sp.get();

            this->AddShaderPackageToMap(packageName, sp);

            return pSp;
        }

        return nullptr;
    }

    // シェーダーパッケージをマップへ追加
    void CShaderManager::AddShaderPackageToMap(const char* name, std::unique_ptr<CShaderPackage>& pSp) {

        if (!GetShaderPackageFromMap(name)) {
            m_pMapShaderPackage[name] = std::move(pSp);
        }
    }

    // シェーダーパッケージを取得
    CShaderPackage* CShaderManager::GetShaderPackageFromMap(const char* name) {
        auto itr = m_pMapShaderPackage.find(name);

        if (itr != m_pMapShaderPackage.end()) {
            return itr->second.get();
        }

        return nullptr;
    }

    // シェーダーパッケージの保存
    bool CShaderManager::SaveShaderPackageList() {

        std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary;

        std::ofstream ofs;
        ofs.open(g_ShaderPackageListFileName, mode);

        if (!ofs.is_open()) return false;

        int packageSize = (UINT)m_pMapShaderPackage.size();
        ofs.write((char*)&packageSize, sizeof(int));

        for (const auto& pSp : m_pMapShaderPackage) {
            if (!pSp.second->SavePackage(ofs)) continue;
        }

        return true;
    }

    // シェーダーパッケージの読み込み
    bool CShaderManager::LoadShaderPackageList() {

        std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary;

        std::ifstream ifs;
        ifs.open(g_ShaderPackageListFileName, mode);

        if (!ifs.is_open()) return false;

        int packageSize = 0;
        ifs.read((char*)&packageSize, sizeof(int));

        for (int i = 0; i < packageSize; ++i) {
            std::unique_ptr<CShaderPackage> sp = std::make_unique<CShaderPackage>();

            if (sp->LoadPackage(ifs)) {
                this->AddShaderPackageToMap(sp->GetPackageName(), sp);
            }
        }

        return true;
    }

} // namespace