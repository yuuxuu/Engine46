/**
 * @file CFileSystem.h
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

    class CFileSystem {
    private:
        std::map<std::string, std::unique_ptr<FileInfo>>    m_pMapFileInfo;

    private:
        CFileSystem();
        ~CFileSystem();

    public:

        bool Initialize();

        FileInfo* CreateFileInfo(const char* filePath);

        void AddFileInfoToMap(const char* name, std::unique_ptr<FileInfo>& pFileInfo);

        FileInfo* GetFileInfoFromMap(const char* fileName);

        bool WriteFile(const char* writeFileName, std::ios::openmode mode, void* pBuffers, size_t size);
        bool ReadFile(const char* readFileName, std::ios::openmode mode, void*& pBuffers, size_t size);

        static CFileSystem& GetFileSystem() {
            static CFileSystem fileSystem;
            return fileSystem;
        }
    };
} // namespace

#endif