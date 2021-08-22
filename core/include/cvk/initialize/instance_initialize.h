#pragma once

#include "cvk/vk_header.h"

#include <string>
#include <vector>

namespace __cvk
{
    void get_all_supported_extensions(std::vector<std::string>& supported_extensions);
    void get_all_supported_layers(std::vector<std::string>& supported_layers);

    VkResult create_instance(const std::vector<std::string>& extensions_name, const std::vector<std::string>& layers_name, VkInstance& instance);
};