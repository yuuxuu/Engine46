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

	class CMeshManager {
	private:
		std::map<const char*, std::unique_ptr<CMeshBase>>	m_pMapMesh;

		CRendererBase*										pRenderer;

	public:
		explicit CMeshManager(CRendererBase* pRenderer);
		~CMeshManager();

		CMeshBase* GetMesh(const char* name);

		CMeshBase* CreateMesh(const char* meshName);

		void AddMeshToMap(const char* name, std::unique_ptr<CMeshBase>& pMesh);
	};

} // namespace

#endif