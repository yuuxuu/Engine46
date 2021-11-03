/**
 * @file CShaderManager.h
 * @brief
 * @author 木村優
 * @date 2020/05/18
 */

#pragma once

#ifndef _CSHADER_MANAGER_H_
#define _CSHADER_MANAGER_H_

namespace Engine46 {

    // 前方宣言
    class CRendererBase;
    class CShaderPackage;
    class CActorBase;

    class CShaderManager {
    private:
        std::map<std::string, std::unique_ptr<CShaderPackage>>  m_pMapShaderPackage;

        CRendererBase* pRenderer;

    public:
        explicit CShaderManager(CRendererBase* pRenderer);
        ~CShaderManager();

        CShaderPackage* CreateShaderPackage(const char* packageName);

        void AddShaderPackageToMap(const char* name, std::unique_ptr<CShaderPackage>& pSp);

        CShaderPackage* GetShaderPackageFromMap(const char* name);

        bool SaveShaderPackageList();
        bool LoadShaderPackageList();
    };

} // namespace

#endif
