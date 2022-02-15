/**
 * @file CMeshManager.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/23
 */

#include "CMeshManager.h"
#include "CMesh.h"
#include "CActor.h"
#include "CModelMesh.h"

#include "CRenderer.h"
#include "CFileManager.h"
#include "CGameSystem.h"
#include "CThreadPoolSystem.h"

#include "CFBXLoader.h"
#include "COBJLoader.h"

namespace Engine46 {

    CFBXLoader fbxLoader;
    COBJLoader objLoader;

    // コンストラクタ
    CMeshManager::CMeshManager(CRendererBase* pRenderer) :
        pRenderer(pRenderer)
    {}

    // デストラクタ
    CMeshManager::~CMeshManager()
    {}

    // メッシュ作成
    CMeshBase* CMeshManager::CreateMesh(const std::string& meshName) {
        CMeshBase* pMesh = GetMeshFromMap(meshName);

        if (pMesh) return pMesh;

        std::unique_ptr<CMeshBase> mesh;
        pRenderer->CreateMesh(mesh, meshName);

        if (mesh) {
            pMesh = mesh.get();

            this->AddMeshToMap(meshName, mesh);

            return pMesh;
        }

        return nullptr;
    }

    // メッシュをマップへ追加
    void CMeshManager::AddMeshToMap(const std::string& name, std::unique_ptr<CMeshBase>& pMesh) {

        if (!GetMeshFromMap(name)) {
            m_pMapMesh[name] = std::move(pMesh);
        }
    }

    // メッシュを取得
    CMeshBase* CMeshManager::GetMeshFromMap(const std::string& name) {
        auto itr = m_pMapMesh.find(name);

        if (itr != m_pMapMesh.end()) {
            return itr->second.get();
        }

        return nullptr;
    }

    // モデルメッシュ作成
    CModelMesh* CMeshManager::CreateModelMesh(const std::string& modelName) {
        CModelMesh* pModelMesh = GetModelMeshFromMap(modelName);

        if (pModelMesh) return pModelMesh;

        std::unique_ptr<CModelMesh> modelMesh = std::make_unique<CModelMesh>(modelName);

        pModelMesh = modelMesh.get();

        FileInfo* pFileInfo = CGameSystem::GetGameSystem().GetFileManager()->GetFileInfoFromMap(modelName);
        if (pFileInfo) {
            std::string ext = pFileInfo->extensionName;
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

            if (ext == ".fbx") {
                std::function<void()> task = [=] {
                    if (!fbxLoader.LoadModel(pModelMesh, pFileInfo->filePath)) {
                        std::string errorStr = pFileInfo->filePath;
                        errorStr += "読み込み：失敗";

                        MessageBox(NULL, errorStr.c_str(), "MessageBox", MB_OK);
                    }
                };

                CThreadPoolSystem::GetThreadPoolSystem().AddWorkTask(task);
            }
            else if (ext == ".obj") {
                std::function<void()> task = [=] {
                    if (!objLoader.LoadModel(pModelMesh, pFileInfo->filePath)) {
                        std::string errorStr = pFileInfo->filePath;
                        errorStr += "読み込み：失敗";

                        MessageBox(NULL, errorStr.c_str(), "MessageBox", MB_OK);
                    }
                };

                CThreadPoolSystem::GetThreadPoolSystem().AddWorkTask(task);
            }
        }

        AddModelMeshToMap(modelName, modelMesh);

        return pModelMesh;
    }

    // モデルメッシュをマップへ追加
    void CMeshManager::AddModelMeshToMap(const std::string& name, std::unique_ptr<CModelMesh>& pModelMesh) {

        if (!GetModelMeshFromMap(name)) {
            m_pMapModelMesh[name] = std::move(pModelMesh);
            return;
        }
    }

    // モデルメッシュを取得
    CModelMesh* CMeshManager::GetModelMeshFromMap(const std::string& name) {
        auto itr = m_pMapModelMesh.find(name);

        if (itr != m_pMapModelMesh.end()) {
            return itr->second.get();
        }

        return nullptr;
    }

} // namespace
