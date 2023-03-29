#pragma once

#ifdef linux

#include "cvk/vk_header.h"
#include "cvk/surface.h"

#include <xcb/xcb.h>

namespace __cvk
{
    CVK_API VkResult create_xcb_surface(VkInstance instance, xcb_connection_t* connection, xcb_window_t window, VkSurfaceKHR& surface);
} // namespace cvk

namespace cvk
{
    
class CVK_API SurfaceXCB : public Surface
{
    using _Base = Surface;

public:
    SurfaceXCB(VkInstance instance, xcb_connection_t* connection, xcb_window_t window);
    virtual ~SurfaceXCB();
};

} // namespace cvk

#endif