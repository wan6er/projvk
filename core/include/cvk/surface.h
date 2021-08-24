#pragma once

#include "cvk/vk_header.h"

namespace cvk
{
    class Surface
    {
    public:
        Surface(VkInstance instance);
        ~Surface();
        
        auto instance() const -> VkInstance;
        auto instance() -> VkInstance&;
        auto surface() const -> VkSurfaceKHR;
        auto surface() -> VkSurfaceKHR&;

        operator VkSurfaceKHR() const;
    
    private:
        VkInstance _instance;
        VkSurfaceKHR _surface = nullptr;
    };
};