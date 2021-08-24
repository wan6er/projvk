#include "cvk/surface_win32.h"

#include "cvk/initialize/win32_initialize.h"

cvk::SurfaceWin32::SurfaceWin32(VkInstance instance, HINSTANCE hInstance, HWND hWnd) :
    Surface(instance)
{

}
cvk::SurfaceWin32::~SurfaceWin32()
{

}