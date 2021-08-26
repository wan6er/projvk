#pragma once

#include "utils/base_obj.h"
#include "cvk/vk_header.h"

namespace cvk
{
    class Memory : public utils::BaseObj<VkDeviceMemory>
    {
    public:
        Memory(VkDevice device, uint32_t size, uint32_t type_index);

        operator VkDeviceMemory() const;

    protected:
        virtual void release();

    private:
        VkDevice _device;
    };
};
