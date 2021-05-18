/**
 * @file CTimer.h
 * @brief
 * @author 木村優
 * @date 2021/05/10
 */

#pragma once

#ifndef _CTIMER_H_
#define _CTIMER_H_

namespace Engine46 {

	class CTimer {
	private:
		DWORD	m_nowTime;
		DWORD	m_oldTime;

	public:
		CTimer();
		~CTimer();

		void Update();
	};

} // namespace

#endif