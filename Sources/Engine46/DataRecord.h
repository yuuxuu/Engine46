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

	class CDataRecordBase {
	protected:
		DATATYPE	m_dataType;
		int			m_dataOffset;
		int			m_dataSize;

	public:
		CDataRecordBase(DATATYPE type, int offset, int size);
		CDataRecordBase(int offset, int size);
		virtual ~CDataRecordBase();

		virtual void WriteData(std::ofstream& ofs, char* p);
		virtual void ReadData(std::ifstream& ifs, char* p);

		DATATYPE GetDataType() const { return m_dataType; }
	};

	class CStrDataRecord : public CDataRecordBase {
	private:
		std::unique_ptr<char[]>& m_pStr;

	public:
		CStrDataRecord(int offset, std::unique_ptr<char[]>& pStr);
		~CStrDataRecord();

		void WriteData(std::ofstream& ofs, char* p) override;
		void ReadData(std::ifstream& ifs, char* p) override;
	};

	class CPtrDataRecord : public CDataRecordBase {
	private:
		int& m_id;

	public:
		CPtrDataRecord(int& id);
		~CPtrDataRecord();

		void WriteData(std::ofstream& ofs, char* p);
		void ReadData(std::ifstream& ifs, char* p);
	};

	class CListDataRecord : public CDataRecordBase {
	private:
		std::vector<int>& m_vecID;

	public:
		CListDataRecord(std::vector<int>& vecID);
		~CListDataRecord();

		void WriteData(std::ofstream& ofs, char* p);
		void ReadData(std::ifstream& ifs, char* p);
	};

} // namespace

#endif
