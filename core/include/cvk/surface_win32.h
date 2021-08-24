#pragma once

#include "cvk/surface.h"

namespace cvk
{
    class SurfaceWin32 : public Surface
    {
    public:
        SurfaceWin32(VkInstance instance, HINSTANCE hInstance, HWND hWnd);
        ~SurfaceWin32();
    };
};