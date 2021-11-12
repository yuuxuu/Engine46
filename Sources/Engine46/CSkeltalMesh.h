/**
 * @file CSkeltalMesh.h
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

    class CSkeltalMesh {
    private:
        std::vector<CMeshBase*> m_pVecMesh;

    public:
        CSkeltalMesh();
        ~CSkeltalMesh();

        void Draw();

        void AddMesh(CMeshBase* pMesh);
    };
} // namespace

#endif
