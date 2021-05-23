/**
 * @file CConstantBuffer.h
 * @brief
 * @author 木村優
 * @date 2021/05/07
 */

#pragma once

#ifndef _CCONSTANTBUFFER_H_
#define _CCONSTANTBUFFER_H_

namespace Engine46 {

	class CConstantBufferBase {
	protected:

	public:
		CConstantBufferBase();
		virtual ~CConstantBufferBase();

		virtual void CreateConstantBuffer(UINT byteWidth) {};
		virtual void Update(void* srcData) {};
		virtual void Set(UINT slot) {};
	};

} // namespace

#endif
