#include "linux/surface_xcb.h"

#ifdef linux
namespace __cvk
{
    
CVK_API VkResult create_xcb_surface(VkInstance instance, xcb_connection_t* connection, xcb_window_t window, VkSurfaceKHR& surface)
{
    VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.connection = connection;
	surfaceCreateInfo.window = window;
	return vkCreateXcbSurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface);
}

} // namespace __cvk

#endif