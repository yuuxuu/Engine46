﻿/**
 * @file CSprite.h
 * @brief
 * @author 木村優
 * @date 2001/05/06
 */

#pragma once

#ifndef _CSPRITE_H_
#define _CSPRITE_H_

#include "CActor.h"

namespace Engine46 {

	class CSprite : public CActorBase {
	private:

	public:
		explicit CSprite(const char* actorName);
		~CSprite();

		void InitializeResource(CRendererBase* pRenderer) override;

		void Update() override;
	};

} // namespace

#endif