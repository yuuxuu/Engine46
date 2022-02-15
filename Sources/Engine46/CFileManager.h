/**
 * @file CFileManager.h
 * @brief
 * @author 木村優
 * @date 2019/01/16
 */

#pragma once

#ifndef _CFILE_SYSTEM_H_
#define _CFILE_SYSTEM_H_

namespace Engine46 {

    constexpr const char* RESOURCE_ROOT_PATH = "D:/Engine46/Assets/";

    struct FileInfo {
        std::string filePath;
        std::string driveName;
        std::string directryName;
        std::string fileName;
        std::string extensionName;

        DWORD lastWriteTime;
        DWORD lastAccessTime;

        FileInfo() :
            lastWriteTime(0),
            lastAccessTime(0)
        {}
    };

    class CFileManager {
    private:
        std::map<std::string, std::unique_ptr<FileInfo>>    m_pMapFileInfo;


    public:
        CFileManager();
        ~CFileManager();

        bool Initialize();

        FileInfo* CreateFileInfo(const std::filesystem::path& filePath);

        void AddFileInfoToMap(const std::string& name, std::unique_ptr<FileInfo>& pFileInfo);

        FileInfo* GetFileInfoFromMap(const std::string& fileName);

        static bool WriteFile(const std::string& writeFileName, std::ios::openmode mode, void* pBuffers, size_t size);
        static bool ReadFile(const std::string& readFileName, std::ios::openmode mode, void*& pBuffers, size_t size);
    };
} // namespace

#endif