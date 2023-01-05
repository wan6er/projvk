#include "win32/surface_win32.h"

#include "cvk/initialize/win32_initialize.h"

cvk::SurfaceWin32::SurfaceWin32(VkInstance instance, HINSTANCE hInstance, HWND hWnd) :
    Surface(instance)
{
    __cvk::create_win32_surface(instance, hInstance, hWnd, object());
}

cvk::SurfaceWin32::~SurfaceWin32()
{

}