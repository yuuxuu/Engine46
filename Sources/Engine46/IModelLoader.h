/**
 * @file IModelLoader.h
 * @brief
 * @author 木村優
 * @date 2021/05/06
 */

#pragma once

#ifndef _IMODEL_LOADER_H_
#define _IMODEL_LOADER_H_

namespace Engine46 {

    // 前方宣言
    class CMeshBase;
    class CMaterialBase;
    class CModelMesh;

    class IModelLoader {
    protected:
        IModelLoader() {}
        virtual ~IModelLoader() {}

        virtual bool LoadModel(CModelMesh* pModel, const std::string& filePath) = 0;

        virtual bool CreateMesh(CMeshBase* pMesh) { return true; };
        virtual bool CreateMaterial(CMaterialBase* pMaterial) { return true; };
    };
} // namespace

#endif
