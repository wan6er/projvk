#include <iostream>

#include "cvk.h"

int main(int argv, char* args[])
{
    std::vector<std::string> extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };
    std::vector<std::string> layers = {
        "VK_LAYER_KHRONOS_validation"
    };
    
    VkInstance instance;
    auto result = __cvk::create_instance(extensions, layers, instance);

    return 0;
}