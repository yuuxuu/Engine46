/**
 * @file DataRecord.h
 * @brief
 * @author 木村優
 * @date 2021/05/01
 */

#pragma once

#ifndef _DATARECORD_H_
#define _DATARECORD_H_

namespace Engine46 {

	enum class DATATYPE {
		TYPE_VAL,
		TYPE_STR,
		TYPE_PTR,
		TYPE_LIST,
		TYPE_END,
	};

	struct DATARECORD {
		DATATYPE dataType;
		int offset;
		int size;

		DATARECORD(DATATYPE dataType, int offset, int size) :
			dataType(dataType),
			offset(offset),
			size(size)
		{}
	};

	struct STR_DATARECORD {
		int offset;
		std::unique_ptr<char[]>& pStr;

		STR_DATARECORD(int offset, std::unique_ptr<char[]>& pStr) :
			offset(offset),
			pStr(pStr)
		{}
		STR_DATARECORD& operator = (const STR_DATARECORD&) { return *this; }
	};
} // namespace

#endif
