#include "linux/surface_xcb.h"

#ifdef linux

namespace cvk
{

SurfaceXCB::SurfaceXCB(VkInstance instance, xcb_connection_t* connection, xcb_window_t window) :
    _Base(instance)
{
    __cvk::create_xcb_surface(instance, connection, window, object());
}

SurfaceXCB::~SurfaceXCB()
{
}

} // namespace cvk


#endif