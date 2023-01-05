#pragma once

#include "cvk/vk_header.h"

namespace __cvk
{
    CVK_API VkResult create_win32_surface(VkInstance instance, HINSTANCE hInstance, HWND hWnd, VkSurfaceKHR& surface);
};