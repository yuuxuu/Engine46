/**
 * @file utility.h
 * @brief
 * @author –Ø‘º—D
 * @date 2021/04/10
 */

#pragma once

#ifndef _UTILITY_H_
#define _UTILITY_H_

namespace Engine46 {
	// vector‚ÌƒNƒŠƒA
	template<class T>
	void VecClear(std::vector<T>& vec) {
		if (!vec.empty()) {
			vec.clear();
			vec.shrink_to_fit();
		}
	}

	// vector‚ÌŠJ•ú
	template <class T>
	void VecRelease(std::vector<T*>& vec) {
		if (!vec.empty()) {
			for (auto i = 0; i < (int)vec.size(); ++i) {
				delete vec[i];
				vec[i] = nullptr;
			}
			VecClear(vec);
		}
	}

	void SaveToBmpFile(BITMAPINFOHEADER m_bmpInfo, LPCVOID pBuffer, std::string fileName);

	std::wstring StringConvertToWString(std::string str);
} // namespace

#endif