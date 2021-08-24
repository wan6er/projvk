#include "cvk/initialize/surface_initialize.h"

void __cvk::destroy_surface(VkInstance instance, VkSurfaceKHR surface)
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
}