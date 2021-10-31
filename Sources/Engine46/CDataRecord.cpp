/**
 * @file DataRecord.cpp
 * @brief
 * @author 木村優
 * @date 2021/05/02
 */

#include "CDataRecord.h"

namespace Engine46 {

    // コンストラクタ
    CDataRecordBase::CDataRecordBase(DATA_TYPE type, int offset, int size) :
        m_dataType(type),
        m_dataOffset(offset),
        m_dataSize(size)
    {}

    // コンストラクタ
    CDataRecordBase::CDataRecordBase(int offset, int size) :
        m_dataType(DATA_TYPE::TYPE_VAL),
        m_dataOffset(offset),
        m_dataSize(size)
    {}

    // デストラクタ
    CDataRecordBase::~CDataRecordBase()
    {}

    // データの書き込み
    void CDataRecordBase::WriteData(std::ofstream& ofs, char* p) {
        ofs.write(p + m_dataOffset, m_dataSize);
    }

    // データの読み込み
    void CDataRecordBase::ReadData(std::ifstream& ifs, char* p) {
        ifs.read(p + m_dataOffset, m_dataSize);
    }

    // コンストラクタ
    CStrDataRecord::CStrDataRecord(int offset, std::string& string) :
        CDataRecordBase(DATA_TYPE::TYPE_STR, offset, (UINT)string.size() + 1),
        m_string(string)
    {}

    // デストラクタ
    CStrDataRecord::~CStrDataRecord()
    {}

    // データの書き込み
    void CStrDataRecord::WriteData(std::ofstream& ofs, char* p) {
        ofs.write((char*)&m_dataSize, sizeof(int));

        ofs.write(m_string.data(), m_dataSize);
    }

    // データの読み込み
    void CStrDataRecord::ReadData(std::ifstream& ifs, char* p) {
        ifs.read((char*)&m_dataSize, sizeof(int));

        ifs.read(m_string.data(), m_dataSize);
    }

    // コンストラクタ
    CPtrDataRecord::CPtrDataRecord(int& id) :
        CDataRecordBase(DATA_TYPE::TYPE_PTR, 0, sizeof(int)),
        m_id(id)
    {}

    // デストラクタ
    CPtrDataRecord::~CPtrDataRecord()
    {}

    // データの書き込み
    void CPtrDataRecord::WriteData(std::ofstream& ofs, char* p) {
        ofs.write((char*)&m_id, m_dataSize);
    }

    // データの読み込み
    void CPtrDataRecord::ReadData(std::ifstream& ifs, char* p) {
        ifs.read((char*)&m_id, m_dataSize);
    }

    // コンストラクタ
    CListDataRecord::CListDataRecord(std::vector<int>& vecID) :
        CDataRecordBase(DATA_TYPE::TYPE_LIST, 0, sizeof(int)),
        m_vecID(vecID)
    {}

    // デストラクタ
    CListDataRecord::~CListDataRecord()
    {}

    // データの書き込み
    void CListDataRecord::WriteData(std::ofstream& ofs, char* p) {
        int size = (int)m_vecID.size();
        ofs.write((char*)&size, m_dataSize);

        for (const auto id : m_vecID) {
            ofs.write((char*)&id, m_dataSize);
        }
    }

    // データの読み込み
    void CListDataRecord::ReadData(std::ifstream& ifs, char* p) {
        int size = -1;
        ifs.read((char*)&size, m_dataSize);

        for (int i = 0; i < size; ++i) {
            int id = 0;
            ifs.read((char*)&id, m_dataSize);

            m_vecID.emplace_back(id);
        }
    }

    // コンストラクタ
    CBufDataRecord::CBufDataRecord(std::unique_ptr<BYTE[]>& pBuf, int& bufSize) :
        CDataRecordBase(DATA_TYPE::TYPE_BUF, 0, 0),
        m_pBuf(pBuf),
        m_byteSize(bufSize)
    {}

    // デストラクタ
    CBufDataRecord::~CBufDataRecord()
    {}

    // データの書き込み
    void CBufDataRecord::WriteData(std::ofstream& ofs, char* p) {
        ofs.write((char*)m_pBuf.get(), m_byteSize);
    }

    // データの読み込み
    void CBufDataRecord::ReadData(std::ifstream& ifs, char* p) {
        m_pBuf.reset(new BYTE[m_byteSize]);

        ifs.read((char*)m_pBuf.get(), m_byteSize);
    }

} // namespace