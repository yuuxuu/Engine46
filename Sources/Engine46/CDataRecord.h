/**
 * @file CDataRecord.h
 * @brief
 * @author 木村優
 * @date 2021/05/01
 */

#pragma once

#ifndef _DATARECORD_H_
#define _DATARECORD_H_

namespace Engine46 {

	enum class DATA_TYPE {
		TYPE_VAL,
		TYPE_STR,
		TYPE_PTR,
		TYPE_LIST,
		TYPE_BUF,
		TYPE_END,
	};

	class CDataRecordBase {
	protected:
		DATA_TYPE	m_dataType;
		int			m_dataOffset;
		int			m_dataSize;

	public:
		CDataRecordBase(DATA_TYPE type, int offset, int size);
		CDataRecordBase(int offset, int size);
		virtual ~CDataRecordBase();

		virtual void WriteData(std::ofstream& ofs, char* p);
		virtual void ReadData(std::ifstream& ifs, char* p);

		DATA_TYPE GetDataType() const { return m_dataType; }
	};

	class CStrDataRecord : public CDataRecordBase {
	private:
		std::string& m_string;

	public:
		CStrDataRecord(int offset, std::string& pStr);
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

	class CBufDataRecord : public CDataRecordBase {
	private:
		std::unique_ptr<BYTE[]>&	m_pBuf;
		int&						m_bufSize;

	public:
		CBufDataRecord(std::unique_ptr<BYTE[]>& pBuf, int& bufSize);
		~CBufDataRecord();

		void WriteData(std::ofstream& ofs, char* p);
		void ReadData(std::ifstream& ifs, char* p);
	};

} // namespace

#endif