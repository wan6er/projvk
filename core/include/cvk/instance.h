#pragma once

#include "utils/base_obj.h"
#include "cvk/vk_header.h"

#include <vector>
#include <string>

namespace cvk
{
    class Instance : public utils::BaseObj<VkInstance>
    {
    public:
        Instance(const std::vector<std::string>& extensions_name, const std::vector<std::string>& layers_name);
        ~Instance();

        auto get_all_physical_device() const -> std::vector<VkPhysicalDevice>;

        operator VkInstance() const;

    protected:
        virtual void release();
    };
};