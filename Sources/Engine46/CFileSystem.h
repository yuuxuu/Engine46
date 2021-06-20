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

	struct FileInfo {
		std::unique_ptr<char[]> filePath;
		std::unique_ptr<char[]> driveName;
		std::unique_ptr<char[]> directryName;
		std::unique_ptr<char[]> fileName;
		std::unique_ptr<char[]> extensionName;

		DWORD lastWriteTime;
		DWORD lastAccessTime;

		FileInfo() :
			lastWriteTime(0),
			lastAccessTime(0)
		{}
	};

	class CFileSystem {
	private:
		std::map<std::string, std::unique_ptr<FileInfo>>	m_pMapFileInfo;

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