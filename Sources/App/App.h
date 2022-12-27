/**
 * @file App.h
 * @brief
 * @author 木村優
 * @date 2022/12/15
 */

#pragma once

#ifndef _App_H_
#define _App_H_

#include <windows.h>

namespace App {

    extern "C" {
        __declspec(dllexport) void AppInitialize(HWND hwnd, int width, int height);

        __declspec(dllexport) void AppFinalize();
    }
} // namespace

#endif