﻿/**
 * @file CInput.h
 * @brief
 * @author 木村優
 * @date 2021/05/11
 */

#pragma once

#ifndef _CINPUT_H_
#define _CINPUT_H_

#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using Microsoft::WRL::ComPtr;

namespace Engine46 {

    constexpr int MAX_KEY = 256;

    class CInput {
    private:
        HWND                            m_hWnd;

        ComPtr<IDirectInput8A>          m_pDirectInput;

        ComPtr<IDirectInputDevice8A>    m_pDirectDeviceKeyboard;
        ComPtr<IDirectInputDevice8A>    m_pDirectDeviceMouse;

        std::array<BYTE, MAX_KEY>       m_key;
        std::array<BYTE, MAX_KEY>       m_oldKey;

        DIMOUSESTATE                    m_mouse;
        DIMOUSESTATE                    m_oldMouse;

        POINT                           m_mousePos;

        bool                            m_isUpdate;

    private:
        bool InitializeKeyBoard(HWND hwnd);
        bool InitializeMouse(HWND hwnd);

        bool UpdateKeyBoard();
        bool UpdateMouse();
    public:
        explicit CInput(HWND hwnd);
        ~CInput();

        bool Initialize(HINSTANCE hInstance);

        void UpdateInput();

        void ChangeUpdateState(bool state);

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
