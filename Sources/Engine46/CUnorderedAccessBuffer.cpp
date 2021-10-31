/**
 * @file CUnorderedAccessBuffer.cpp
 * @brief
 * @author 木村優
 * @date 2021/09/29
 */

#include "CUnorderedAccessBuffer.h"

namespace Engine46 {

    // コンストラクタ
    CUnorderedAccessBufferBase::CUnorderedAccessBufferBase() :
        m_byteWidth(0),
        m_byteSize(0),
        m_texWidth(0),
        m_texHeight(0)
    {}

    // デストラクタ
    CUnorderedAccessBufferBase::~CUnorderedAccessBufferBase()
    {}

} // namespace
