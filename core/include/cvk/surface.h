#pragma once

#include "utils/base_obj.h"
#include "cvk/vk_header.h"

namespace cvk
{
    class CVK_API Surface : protected utils::BaseObj<VkSurfaceKHR>
    {
    public:
        Surface(VkInstance instance);
        ~Surface();
        
        auto instance() const -> VkInstance;
        auto instance() -> VkInstance&;
        auto surface() const -> VkSurfaceKHR;
        auto surface() -> VkSurfaceKHR&;

        operator VkSurfaceKHR() const;
    
    protected:
        void release();

    private:
        VkInstance _instance = nullptr;
    };
};