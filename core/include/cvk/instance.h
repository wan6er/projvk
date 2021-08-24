#pragma once

#include "cvk/vk_header.h"

#include <vector>
#include <string>

namespace cvk
{
    class Instance
    {
    public:
        Instance(const std::vector<std::string>& extensions_name, const std::vector<std::string>& layers_name);
        ~Instance();

        auto get_all_physical_device() const -> std::vector<VkPhysicalDevice>;

        operator VkInstance() const;
    private:
        VkInstance _instance;
    };
};