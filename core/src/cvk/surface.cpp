#include "cvk/surface.h"

#include "cvk/initialize/surface_initialize.h"

cvk::Surface::Surface(VkInstance instance) :
    _instance(instance)
{

}
cvk::Surface::~Surface()
{
    if (_surface) {
        __cvk::destroy_surface(_instance, _surface);
    }
}

auto cvk::Surface::instance() const -> VkInstance
{
    return _instance;
}

auto cvk::Surface::instance() -> VkInstance&
{
    return _instance;
}

auto cvk::Surface::surface() const -> VkSurfaceKHR
{
    return _surface;
}

auto cvk::Surface::surface() -> VkSurfaceKHR &
{
    return _surface;
}

cvk::Surface::operator VkSurfaceKHR() const
{
    return _surface;
}