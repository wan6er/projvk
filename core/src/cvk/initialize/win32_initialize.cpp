#include "cvk/initialize/win32_initialize.h"

namespace __cvk
{


VkResult create_win32_surface(VkInstance instance, HINSTANCE hInstance, HWND hWnd, VkSurfaceKHR& surface)
{
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = (HINSTANCE)hInstance;
	surfaceCreateInfo.hwnd = (HWND)hWnd;
	return vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface);
}

};