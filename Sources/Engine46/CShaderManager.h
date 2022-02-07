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

    class CShaderManager {
    private:
        std::map<std::string, std::unique_ptr<CShaderPackage>>  m_pMapShaderPackage;

        CRendererBase* pRenderer;

    public:
        explicit CShaderManager(CRendererBase* pRenderer);
        ~CShaderManager();

        CShaderPackage* CreateShaderPackage(const std::string& packageName);

        void AddShaderPackageToMap(const std::string& name, std::unique_ptr<CShaderPackage>& pSp);

        CShaderPackage* GetShaderPackageFromMap(const std::string& name);

        bool SaveShaderPackageList();
        bool LoadShaderPackageList();
    };

} // namespace

#endif
