#pragma once

#include "cvk/vk_header.h"

#include <string>
#include <vector>

#define CVK_GET_INSTANCE_PROC_ADDR(instance, proc_name) reinterpret_cast<PFN_##proc_name>(vkGetInstanceProcAddr(instance, #proc_name))

namespace __cvk
{
    VkResult create_instance(const std::vector<std::string>& extensions_name, const std::vector<std::string>& layers_name, VkInstance& instance);
    void destroy_instance(VkInstance instance);

    void get_all_devices(VkInstance instance, std::vector<VkPhysicalDevice>& devices);
    void get_all_instance_extensions(std::vector<std::string>& supported_extensions);
    void get_all_instance_layers(std::vector<std::string>& supported_layers);
};