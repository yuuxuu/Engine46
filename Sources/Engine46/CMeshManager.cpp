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
#include "CFileSystem.h"

#include "CFBXLoader.h"

namespace Engine46 {

    // コンストラクタ
    CMeshManager::CMeshManager(CRendererBase* pRenderer) :
        pRenderer(pRenderer)
    {}

    // デストラクタ
    CMeshManager::~CMeshManager()
    {}

    // メッシュ作成
    CMeshBase* CMeshManager::CreateMesh(const char* meshName) {
        CMeshBase* pMesh = GetMeshFromMap(meshName);

        std::string name(meshName);
        if (pMesh) {
            int count = 0;
            for (;;) {
                name = std::string(meshName) + "_" + std::to_string(count++);

                if (!GetMeshFromMap(name.c_str())) {
                    break;
                }
            }
        }

        std::unique_ptr<CMeshBase> mesh;

        pRenderer->CreateMesh(mesh, name.c_str());

        if (mesh) {
            pMesh = mesh.get();

            this->AddMeshToMap(name.c_str(), mesh);

            return pMesh;
        }

        return nullptr;
    }

    // メッシュをマップへ追加
    void CMeshManager::AddMeshToMap(const char* name, std::unique_ptr<CMeshBase>& pMesh) {

        if (!GetMeshFromMap(name)) {
            m_pMapMesh[name] = std::move(pMesh);
        }
    }

    // メッシュを取得
    CMeshBase* CMeshManager::GetMeshFromMap(const char* name) {
        auto itr = m_pMapMesh.find(name);

        if (itr != m_pMapMesh.end()) {
            return itr->second.get();
        }

        return nullptr;
    }

    // モデルメッシュ作成
    CModelMesh* CMeshManager::CreateModelMesh(const char* modelName) {
        CModelMesh* pModelMesh = GetModelMeshFromMap(modelName);

        if (pModelMesh) return pModelMesh;

        std::unique_ptr<CModelMesh> modelMesh = std::make_unique<CModelMesh>(modelName);

        FileInfo* pFileInfo = CFileSystem::GetFileSystem().GetFileInfoFromMap(modelName);
        if (pFileInfo) {
            std::string ext = pFileInfo->extensionName;
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            if (ext == ".fbx") {
                CFBXLoader fbxLoader;
                fbxLoader.LoadModel(modelMesh.get(), pFileInfo->filePath);
            }
        }

        pModelMesh = modelMesh.get();

        AddModelMeshToMap(modelName, modelMesh);

        return pModelMesh;
    }

    // モデルメッシュをマップへ追加
    void CMeshManager::AddModelMeshToMap(const char* name, std::unique_ptr<CModelMesh>& pModelMesh) {

        if (!GetModelMeshFromMap(name)) {
            m_pMapModelMesh[name] = std::move(pModelMesh);
            return;
        }
    }

    // モデルメッシュを取得
    CModelMesh* CMeshManager::GetModelMeshFromMap(const char* name) {
        auto itr = m_pMapModelMesh.find(name);

        if (itr != m_pMapModelMesh.end()) {
            return itr->second.get();
        }

        return nullptr;
    }

} // namespace
