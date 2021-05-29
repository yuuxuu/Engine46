/**
 * @file CMaterialManager.h
 * @brief
 * @author 木村優
 * @date 2021/05/23
 */

#pragma once

#ifndef _CMATERAIL_MANAGER_H_
#define _CMATERAIL_MANAGER_H_

namespace Engine46 {

	// 前方宣言
	class CRendererBase;
	class CMaterialBase;

	class CMaterialManager {
	private:
		std::map<const char*, std::unique_ptr<CMaterialBase>>	m_pMapMaterial;

		CRendererBase*											pRenderer;

	public:
		explicit CMaterialManager(CRendererBase* pRenderer);
		~CMaterialManager();

		CMaterialBase* GetMaterial(const char* name);

		CMaterialBase* CreateMaterial(const char* materialName);

		void AddMaterialToMap(const char* name, std::unique_ptr<CMaterialBase>& pMaterial);
	};

} // namespace

#endif