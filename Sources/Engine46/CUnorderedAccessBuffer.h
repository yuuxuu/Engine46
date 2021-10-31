/**
 * @file CUnorderedAccessBuffer.cpp
 * @brief
 * @author 木村優
 * @date 2021/09/29
 */

#pragma once

#ifndef _CUNORDERERD_ACCESS_BUFFER_H_
#define _CUNORDERERD_ACCESS_BUFFER_H_

namespace Engine46 {

    class CUnorderedAccessBufferBase {
    protected:
        UINT        m_byteWidth;
        UINT        m_byteSize;
                    
        UINT        m_texWidth;
        UINT        m_texHeight;

        DXGI_FORMAT m_texFormat;

    public:
        CUnorderedAccessBufferBase();
        virtual ~CUnorderedAccessBufferBase();

        virtual void CreateUnorderedAccessBuffer(UINT byteWidth, UINT byteSize) {};
        virtual void CreateUnorderedAccessBufferView() {};
        virtual void Set(UINT slot) {};
        virtual void SetCompute(UINT slot) {};
        virtual void WirteBufferData(void* srcData) {};

        UINT GetTextureWidth() const { return m_texWidth; }
        UINT GetTextureHeight() const { return m_texHeight; }
    };

} // namespace

#endif
