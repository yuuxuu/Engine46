/**
 * @file CFileSystem.cpp
 * @brief
 * @author 木村優
 * @date 2019/01/16
 */

#include "CFileSystem.h"

namespace Engine46 {

	// コンストラクタ
	CFileSystem::CFileSystem()
	{}

	// デストラクタ
	CFileSystem::~CFileSystem()
	{}

	// 初期化
	bool CFileSystem::Initialize() {

		const std::vector<const char*> vecDirName = {
			"D:/Engine46/Assets/"
		};

		for (const auto name : vecDirName) {
			std::filesystem::recursive_directory_iterator itr = std::filesystem::recursive_directory_iterator(name);

			for (const auto& entry : itr) {
				FileInfo* fileInfo = CreateFileInfo(entry.path().string().c_str());
			}
		}

		return true;
	}

	// ファイル情報を作成
	FileInfo* CFileSystem::CreateFileInfo(const char* filePath) {
		const UINT charSize = 128;

		char drive[charSize];
		char dir[charSize];
		char name[charSize];
		char extension[charSize];

		std::string str;

		_splitpath_s(filePath, drive, dir, name, extension);

		// 拡張子が無ければディレクトリー
		if (strlen(extension) == 0) return nullptr;

		FileInfo* pFileInfo = GetFileInfoFromMap(name);

		if (!pFileInfo) {
			std::unique_ptr<FileInfo> fileInfo = std::make_unique<FileInfo>();

			size_t size = strlen(filePath) + 1;
			fileInfo->filePath.reset(new char[size]);
			std::memcpy(fileInfo->filePath.get(), filePath, size);

			size = strlen(drive) + 1;
			fileInfo->driveName.reset(new char[size]);
			std::memcpy(fileInfo->driveName.get(), drive, size);

			size = strlen(dir) + 1;
			fileInfo->directryName.reset(new char[size]);
			std::memcpy(fileInfo->directryName.get(), dir, size);

			size = strlen(name) + strlen(extension) + 1;
			fileInfo->fileName.reset(new char[size]);
			strcat_s(name, size, extension);
			std::memcpy(fileInfo->fileName.get(), name, size);

			size = strlen(extension) + 1;
			fileInfo->extensionName.reset(new char[size]);
			std::memcpy(fileInfo->extensionName.get(), extension, size);

			SYSTEMTIME sysTime;
			WIN32_FIND_DATA findData;
			HANDLE handle;

			handle = FindFirstFile(filePath, &findData);
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
	void CFileSystem::AddFileInfoToMap(const char* name, std::unique_ptr<FileInfo>& pFileInfo) {

		if (!GetFileInfoFromMap(name)) {
			m_pMapFileInfo[pFileInfo->fileName.get()] = std::move(pFileInfo);
		}
	}

	// ファイル情報を取得
	FileInfo* CFileSystem::GetFileInfoFromMap(const char* fileName) {
		auto itr = m_pMapFileInfo.find(fileName);

		if (itr != m_pMapFileInfo.end()) {
			return itr->second.get();
		}

		return nullptr;
	}

	// ファイルの書き込み
	bool CFileSystem::WriteFile(const char* writeFileName, std::ios::openmode mode, void* pBuffers, size_t size) {
		std::ofstream ofs;

		ofs.open(writeFileName, mode);

		if (!ofs.is_open()) return false;

		ofs.write((char*)pBuffers, size);

		return true;
	}

	// ファイルの読み込み
	bool CFileSystem::ReadFile(const char* readFileName, std::ios::openmode mode, void*& pBuffers, size_t size) {
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
