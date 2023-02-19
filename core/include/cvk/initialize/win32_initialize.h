#pragma once

#include "cvk/vk_header.h"

#if defined(WIN32)

namespace __cvk
{
    CVK_API VkResult create_win32_surface(VkInstance instance, HINSTANCE hInstance, HWND hWnd, VkSurfaceKHR& surface);
};

#endif