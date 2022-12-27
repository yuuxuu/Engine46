/**
 * @file App.cpp
 * @brief
 * @author 木村優
 * @date 2022/12/15
 */

#include "App.h"
#include "main .h"

#pragma comment(lib, "Engine46.lib")

namespace App {

    void AppInitialize(HWND hwnd, int width, int height) {
        Engine46::Initialize(hwnd, width, height);
    }

    void AppFinalize() {
        Engine46::Finalize();
    }
} // namespace