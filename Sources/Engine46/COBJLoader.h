/**
 * @file COBJLoader.h
 * @brief
 * @author 木村優
 * @date 2022/01/30
 */

#pragma once

#ifndef _COBJ_LOADER_H_
#define _COBJ_LOADER_H_

#include "IModelLoader.h"

namespace Engine46 {

    // 前方宣言
    class CMeshBase;
    class CMaterialBase;

    class COBJLoader : public IModelLoader {
    public:
        COBJLoader();
        ~COBJLoader();

        bool LoadModel(CModelMesh* pModel, const std::string& filePath) override;

        bool LoadMesh(CModelMesh* pModel, std::ifstream& ifs);
        bool LoadMaterial(std::ifstream& ifs);
    };

} // namespace

#endif
