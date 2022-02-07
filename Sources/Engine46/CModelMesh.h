/**
 * @file CModelMesh.h
 * @brief
 * @author 木村優
 * @date 2021/05/06
 */

#pragma once

#ifndef _CMODEL_H_
#define _CMODEL_H_

namespace Engine46 {

    // 前方宣言
    class CMeshBase;
    class CMaterialBase;

    class CModelMesh {
    private:
        std::vector<CMeshBase*> m_pVecMesh;

        std::string             m_modelName;

    public:
        explicit CModelMesh(const std::string& modelName);
        ~CModelMesh();

        void Draw();

        void AddMesh(CMeshBase* pMesh);

        std::vector<CMeshBase*> GetVecMesh() const { return m_pVecMesh; }
    };
} // namespace

#endif
