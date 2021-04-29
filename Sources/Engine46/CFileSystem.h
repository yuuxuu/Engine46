/**
 * @file CFileSystem.h
 * @brief
 * @author –Ø‘º—D
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

		bool WriteFile(std::string writeFileName, std::ios::openmode mode, LPVOID pBuffers, size_t size);
		bool ReadFile(std::string readFileName, std::ios::openmode mode, LPVOID& pBuffers, size_t size);
	};

} // namespace

#endif