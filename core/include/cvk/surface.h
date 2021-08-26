#pragma once

#include "utils/base_obj.h"
#include "cvk/vk_header.h"

namespace cvk
{
    class Surface : public utils::BaseObj<VkSurfaceKHR>
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
        virtual void release();

    private:
        VkInstance _instance = nullptr;
    };
};