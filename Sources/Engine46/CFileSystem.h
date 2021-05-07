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