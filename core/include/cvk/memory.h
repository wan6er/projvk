#pragma once

#include "utils/base_obj.h"
#include "cvk/vk_header.h"

namespace cvk
{
    class CVK_API Memory : protected utils::BaseObj<VkDeviceMemory>
    {
    public:
        Memory(VkDevice device);
        Memory(VkDeviceMemory CONST_REFERENCE memory);
        Memory(Memory CONST_REFERENCE) = default;
        ~Memory();

        operator VkDeviceMemory CONST_REFERENCE () const;

        VkResult bind(VkBuffer buffer, uint32_t offset = 0); 
        VkResult bind(VkImage image, uint32_t offset = 0); 
        
        template<class _Type>
        VkResult map(_Type&& data, size_t offset = 0);
        void unmap();

        VkResult upload(void CONST_PTR data, size_t size, size_t offset = 0);

        virtual VkResult allocate(VkPhysicalDeviceMemoryProperties CONST_REFERENCE properties, VkMemoryRequirements CONST_REFERENCE requirements, VkMemoryPropertyFlags property);
        virtual VkResult allocate(uint32_t size, uint32_t type_index);

        auto allocate_info() const -> VkMemoryAllocateInfo CONST_REFERENCE;
        auto allocate_info() -> VkMemoryAllocateInfo&;

        size_t get_size() const;

    protected:
        void release();

    private:
        VkDevice _device;
        VkMemoryAllocateInfo _info;
    };
};

#include "memory.inl"