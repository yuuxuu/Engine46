/**
 * @file CObject.h
 * @brief
 * @author –Ø‘º—D
 * @date 2018/12/18
 */

#ifndef _COBJECT_H_
#define _COBJECT_H_

#pragma once

#include "math.h"

namespace Engine46 {

	enum class DATATYPE {
		TYPE_VAL,
		TYPE_STR,
		TYPE_PTR,
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
	
	class CObject {
	protected:
		static std::vector<DATARECORD>	m_vecDataRecords;

		std::vector<STR_DATARECORD>		m_vecStrDataRecords;

		UINT							m_ClassID;

		CObject*						pParent;
		CObject*						pChiled;

		int								m_ObjectID;

		std::unique_ptr<char[]>			m_Name;

		Transform						m_Transform;

	public:
		CObject();
		CObject(UINT id, const char* name, const Transform transform);
		virtual ~CObject();

		void ConnectParent(CObject* pObject) { pParent = pObject; }
		CObject* GetParentObject() const { return pParent; }
		
		void ConnectChiled(CObject* pObject) { pChiled = pObject; }
		CObject* GetChiledObject() const { return pChiled; }

		virtual void Initilaize(const char* pStr= nullptr);

		virtual void Update();
		virtual void Draw();

		virtual const char* GetStrFromMemberVariable(int offset);
		virtual void SetStrToMemberVariable(const char* pStr,int size, int offset);

		DATARECORD* GetObjectDataRecords() { return m_vecDataRecords.data(); }
	};
} // namespace

#endif