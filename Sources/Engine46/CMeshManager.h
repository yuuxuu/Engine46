/**
 * @file CMeshManager.h
 * @brief
 * @author 木村優
 * @date 2021/05/23
 */

#pragma once

#ifndef _CMESH_MANAGER_H_
#define _CMESH_MANAGER_H_

namespace Engine46 {

    // 前方宣言
    class CRendererBase;
    class CMeshBase;
    class CModelMesh;

    class CMeshManager {
    private:
        std::map<std::string, std::unique_ptr<CMeshBase>>   m_pMapMesh;
        std::map<std::string, std::unique_ptr<CModelMesh>>  m_pMapModelMesh;

        CRendererBase* pRenderer;

    public:
        explicit CMeshManager(CRendererBase* pRenderer);
        ~CMeshManager();

        CMeshBase* CreateMesh(const char* meshName);

        void AddMeshToMap(const char* name, std::unique_ptr<CMeshBase>& pMesh);

        CMeshBase* GetMeshFromMap(const char* name);

        CModelMesh* CreateModelMesh(const char* modelName);

        void AddModelMeshToMap(const char* name, std::unique_ptr<CModelMesh>& pModelMesh);

        CModelMesh* GetModelMeshFromMap(const char* name);
    };

} // namespace

#endif