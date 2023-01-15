#pragma once

#include "cvk/vk_header.h"

#include <string>
#include <vector>

#define CVK_GET_INSTANCE_PROC_ADDR(instance, proc_name) reinterpret_cast<PFN_##proc_name>(vkGetInstanceProcAddr(instance, #proc_name))

namespace __cvk
{
    CVK_API VkResult create_instance(std::vector<std::string> CONST_REFERENCE extensions_name, std::vector<std::string> CONST_REFERENCE layers_name, VkInstance& instance);
    CVK_API void destroy_instance(VkInstance instance);

    CVK_API void get_all_devices(VkInstance instance, std::vector<VkPhysicalDevice>& devices);
    CVK_API void get_all_instance_extensions(std::vector<std::string>& supported_extensions);
    CVK_API void get_all_instance_layers(std::vector<std::string>& supported_layers);
};