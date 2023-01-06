#pragma once

#include "utils/base_obj.h"
#include "cvk/vk_header.h"

namespace cvk
{
    class CVK_API Memory : protected utils::BaseObj<VkDeviceMemory>
    {
    public:
        Memory(VkDevice device, VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryRequirements CONST_REFERENCE requirements, VkMemoryPropertyFlags property);
        Memory(VkDevice device, uint32_t size, uint32_t type_index);
        Memory(VkDeviceMemory CONST_REFERENCE memory);
        Memory(Memory CONST_REFERENCE) = default;
        ~Memory();

        operator VkDeviceMemory CONST_REFERENCE () const;

        template<class _Type>
        VkResult bind(_Type&& obj, uint32_t offset = 0); 
        
        template<class _Type>
        VkResult map(_Type&& data, uint32_t offset = 0);
        void unmap();

        VkResult allocate();
        auto allocate_info() -> VkMemoryAllocateInfo&;

    protected:
        void release();

    private:
        VkDevice _device;
        VkMemoryAllocateInfo _info;
    };
};

#include "memory.inl"