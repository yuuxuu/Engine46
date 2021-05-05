/**
 * @file utility.h
 * @brief
 * @author 木村優
 * @date 2021/04/10
 */

#pragma once

#ifndef _UTILITY_H_
#define _UTILITY_H_

namespace Engine46 {
	// vectorのクリア
	template<class T>
	void VecClear(std::vector<T>& vec) {
		if (!vec.empty()) {
			vec.clear();
			vec.shrink_to_fit();
		}
	}

	// vectorの開放
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

	void SaveToBmpFile(BITMAPINFOHEADER m_bmpInfo, LPCVOID pStr, const char* fileName);

	wchar_t* CharConvertToWchar(const char* pSrc);
} // namespace

#endif