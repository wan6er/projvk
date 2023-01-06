#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"
#include "utils/base_multi_attach.h"

namespace cvk
{
    class CVK_API DescriptorPool :
        protected utils::BaseObj<VkDescriptorPool>,
        protected utils::BaseMultipleAttaches<VkDescriptorPoolSize>
    {
    public:
        DescriptorPool(VkDevice device);
        DescriptorPool(DescriptorPool CONST_REFERENCE) = default;
        virtual ~DescriptorPool();

        virtual VkResult create(uint32_t max_sets);
        void set(VkDescriptorSetLayoutBinding CONST_REFERENCE layout_binding);
        void set(VkDescriptorType type, uint32_t num_of_descriptor);
        uint32_t& get(VkDescriptorType type);

        operator VkDescriptorPool CONST_REFERENCE () const;

    protected:
        VkDevice get_device() const;
        void release();

    private:
        VkDescriptorPoolCreateInfo _create_info = {};
        VkDevice _device = VK_NULL_HANDLE;

    };
};