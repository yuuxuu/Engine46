﻿/**
 * @file CSprite.h
 * @brief
 * @author 木村優
 * @date 2001/05/06
 */

#pragma once

#include "CActor.h"

namespace Engine46 {

	class CSprite : public CActorBase {
	private:


	public:
		CSprite();
		~CSprite();

		void Initialize() override;
	};

} // namespace
