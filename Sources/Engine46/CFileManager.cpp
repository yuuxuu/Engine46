/**
 * @file CFileManager.cpp
 * @brief
 * @author 木村優
 * @date 2019/01/16
 */

#include "CFileManager.h"

namespace Engine46 {

    std::string CFileManager::ResourceRootPath() {
        static std::string path;
        if (!path.empty()) return path;

        path = std::filesystem::current_path().generic_string();
        int pos = static_cast<int>(path.find(PROJECT_ROOT_PATH));
        if (pos != std::string::npos) {
            path = path.substr(0, pos);
            path += std::string(PROJECT_ROOT_PATH) + std::string(RESOURCE_ROOT_PATH);
        }
        return path;
    }

    // コンストラクタ
    CFileManager::CFileManager()
    {}

    // デストラクタ
    CFileManager::~CFileManager()
    {}

    // 初期化
    bool CFileManager::Initialize() {

        auto itr = std::filesystem::recursive_directory_iterator(ResourceRootPath());
        for (const auto& file : itr) {
            if (file.path().extension().empty()) {
                continue;
            }
            FileInfo* fileInfo = CreateFileInfo(file.path());
        }

        return true;
    }

    // ファイル情報を作成
    FileInfo* CFileManager::CreateFileInfo(const std::filesystem::path& filePath) {

        FileInfo* pFileInfo = GetFileInfoFromMap(filePath.filename().string());

        if (!pFileInfo) {
            std::unique_ptr<FileInfo> fileInfo = std::make_unique<FileInfo>();

            fileInfo->filePath = filePath.generic_string();

            fileInfo->driveName = filePath.root_name().string();

            fileInfo->directryName = filePath.parent_path().generic_string();

            fileInfo->fileName = filePath.filename().string();

            fileInfo->extensionName = filePath.extension().string();

            SYSTEMTIME sysTime;
            WIN32_FIND_DATA findData;
            HANDLE handle;

            handle = FindFirstFile(fileInfo->filePath.c_str(), &findData);
            GetFileTime(handle, &findData.ftCreationTime, &findData.ftLastAccessTime, &findData.ftLastWriteTime);

            if (FileTimeToSystemTime(&findData.ftLastWriteTime, &sysTime)) {
                fileInfo->lastWriteTime = sysTime.wYear;
            }
            if (FileTimeToSystemTime(&findData.ftLastAccessTime, &sysTime)) {
                fileInfo->lastAccessTime = sysTime.wYear;
            }

            pFileInfo = fileInfo.get();

            this->AddFileInfoToMap(fileInfo->fileName, fileInfo);
        }

        return pFileInfo;
    }

    // ファイル情報を追加
    void CFileManager::AddFileInfoToMap(const std::string& name, std::unique_ptr<FileInfo>& pFileInfo) {

        if (!GetFileInfoFromMap(name)) {
            m_pMapFileInfo[pFileInfo->fileName] = std::move(pFileInfo);
        }
    }

    // ファイル情報を取得
    FileInfo* CFileManager::GetFileInfoFromMap(const std::string& fileName) {
        auto itr = m_pMapFileInfo.find(fileName);

        if (itr != m_pMapFileInfo.end()) {
            return itr->second.get();
        }

        return nullptr;
    }

    // ファイルの書き込み
    bool CFileManager::WriteFile(const std::string& writeFileName, std::ios::openmode mode, void* pBuffers, size_t size) {
        std::ofstream ofs;

        ofs.open(writeFileName, mode);

        if (!ofs.is_open()) return false;

        ofs.write((char*)pBuffers, size);

        return true;
    }

    // ファイルの読み込み
    bool CFileManager::ReadFile(const std::string& readFileName, std::ios::openmode mode, void*& pBuffers, size_t size) {
        std::ifstream ifs;

        ifs.open(readFileName, mode);

        if (!ifs.is_open()) return false;

        ifs.seekg(0, ifs.end);
        size = static_cast<size_t>(ifs.tellg());
        ifs.seekg(0, ifs.beg);

        pBuffers = new void*[size];

        ifs.read((char*)pBuffers, size);

        return true;
    }

} // namespace
