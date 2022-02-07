/**
 * @file CFileManager.cpp
 * @brief
 * @author 木村優
 * @date 2019/01/16
 */

#include "CFileManager.h"

namespace Engine46 {

    // コンストラクタ
    CFileManager::CFileManager()
    {}

    // デストラクタ
    CFileManager::~CFileManager()
    {}

    // 初期化
    bool CFileManager::Initialize() {

        const std::vector<std::string> vecDirName = {
            RESOURCE_ROOT_PATH
        };

        for (const auto& name : vecDirName) {
            std::filesystem::recursive_directory_iterator itr = std::filesystem::recursive_directory_iterator(name);

            for (const auto& entry : itr) {
                FileInfo* fileInfo = CreateFileInfo(entry.path().string());
            }
        }

        return true;
    }

    // ファイル情報を作成
    FileInfo* CFileManager::CreateFileInfo(const std::string& filePath) {
        const UINT charSize = 128;

        char drive[charSize];
        char dir[charSize];
        char name[charSize];
        char extension[charSize];

        std::string str;

        _splitpath_s(filePath.c_str(), drive, dir, name, extension);

        // 拡張子が無ければディレクトリー
        if (strlen(extension) == 0) return nullptr;

        FileInfo* pFileInfo = GetFileInfoFromMap(name);

        if (!pFileInfo) {
            std::unique_ptr<FileInfo> fileInfo = std::make_unique<FileInfo>();

            fileInfo->filePath = filePath;

            fileInfo->driveName = drive;

            fileInfo->directryName = dir;

            fileInfo->fileName = name;
            fileInfo->fileName += extension;

            fileInfo->extensionName = extension;

            SYSTEMTIME sysTime;
            WIN32_FIND_DATA findData;
            HANDLE handle;

            handle = FindFirstFile(filePath.c_str(), &findData);
            GetFileTime(handle, &findData.ftCreationTime, &findData.ftLastAccessTime, &findData.ftLastWriteTime);

            if (FileTimeToSystemTime(&findData.ftLastWriteTime, &sysTime)) {
                fileInfo->lastWriteTime = sysTime.wYear;
            }
            if (FileTimeToSystemTime(&findData.ftLastAccessTime, &sysTime)) {
                fileInfo->lastAccessTime = sysTime.wYear;
            }

            pFileInfo = fileInfo.get();

            this->AddFileInfoToMap(name, fileInfo);
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
