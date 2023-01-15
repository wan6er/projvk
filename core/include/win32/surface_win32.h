#pragma once

#include "cvk/vk_header.h"
#include "cvk/surface.h"

namespace cvk
{
    class CVK_API SurfaceWin32 : public Surface
    {
    public:
        SurfaceWin32(VkInstance instance, HINSTANCE hInstance, HWND hWnd);
        ~SurfaceWin32();
    };
};
