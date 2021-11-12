/**
 * @file CFBXLoader.h
 * @brief
 * @author 木村優
 * @date 2021/11/07
 */

#pragma once

#ifndef _CFBX_LOADER_H_
#define _CFBX_LOADER_H_

#include "IModelLoader.h"

namespace Engine46 {

    // 前方宣言
    class CMeshBase;
    class CMaterialBase;

    class CFBXLoader : public IModelLoader {
    public:
        CFBXLoader();
        ~CFBXLoader();

        bool LoadModel(CSkeltalMesh* pModel, const std::string& filePath) override;

        void RecursiveFbxNode(std::vector<FbxNode*>& outVecFbxNode, FbxNode* pRootNode);

        CMeshBase* ConvertMesh(FbxMesh* pFbxMesh);
        CMaterialBase* ConvertMaterial(FbxSurfaceMaterial* pFbxMaterialName);
    };
} // namespacce

#endif
