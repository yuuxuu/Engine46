/**
 * @file CFileSystem.h
 * @brief
 * @author 木村優
 * @date 2019/01/16
 */

#ifndef _CFILE_SYSTEM_MANAGER_H_
#define _CFILE_SYSTEM_MANAGER_H_

#pragma once

namespace Engine46 {

	class CFileSystem {
	private:

	public:
		CFileSystem();
		~CFileSystem();

		bool WriteFile(const char* writeFileName, std::ios::openmode mode, void* pBuffers, size_t size);
		bool ReadFile(const char* readFileName, std::ios::openmode mode, void*& pBuffers, size_t size);
	};

} // namespace

#endif