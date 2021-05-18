/**
 * @file CInput.h
 * @brief
 * @author 木村優
 * @date 2021/05/11
 */

#pragma once

#ifndef _CINPUT_H_
#define _CINPUT_H_

using Microsoft::WRL::ComPtr;

namespace Engine46 {

	constexpr int MAX_KEY = 256;

	class CInput {
	private:
		HWND							m_hWnd;

		ComPtr<IDirectInput8A>			m_pDirectInput;

		ComPtr<IDirectInputDevice8A>	m_pDirectDeviceKeyboard;
		ComPtr<IDirectInputDevice8A>	m_pDirectDeviceMouse;

		BYTE							m_key[MAX_KEY];
		BYTE							m_oldKey[MAX_KEY];

		DIMOUSESTATE					m_mouse;
		DIMOUSESTATE					m_oldMouse;

		POINT							m_mousePos;

		bool InitializeKeyBoard(HWND hwnd);
		bool InitializeMouse(HWND hwnd);
		
		bool UpdateKeyBoard();
		bool UpdateMouse();
	public:
		explicit CInput(HWND hwnd);
		~CInput();

		bool Initialize(HINSTANCE hInstance);

		void UpdateInput();

		bool IsPressKey(UINT key);
		bool IsTriggerKey(UINT key);

		bool IsPressLeftMouse();
		bool IsTriggerLeftMouse();

		bool IsPressRightMouse();
		bool IsTriggerRightMouse();

		bool IsPressCenterMouse();
		bool IsTriggerCenterMouse();

		float GetMousePosX() { return (float)m_mouse.lX; }
		float GetMousePosY() { return (float)m_mouse.lY; }
		float GetMousePosZ() { return (float)m_mouse.lZ; }
	};

} // namespace

#endif
