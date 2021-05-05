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
