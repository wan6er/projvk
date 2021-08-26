#include "cvk/surface.h"

#include "cvk/initialize/surface_initialize.h"

cvk::Surface::Surface(VkInstance instance) :
    _instance(instance)
{

}
cvk::Surface::~Surface()
{
}

void cvk::Surface::release()
{
    __cvk::destroy_surface(_instance, object());
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
    return object();
}

auto cvk::Surface::surface() -> VkSurfaceKHR &
{
    return object();
}

cvk::Surface::operator VkSurfaceKHR() const
{
    return object();
}