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
        explicit DescriptorPool(uint32_t max_sets);
        DescriptorPool(DescriptorPool CONST_REFERENCE) = default;
        virtual ~DescriptorPool();

        VkResult create(VkDevice device);
        void set(VkDescriptorSetLayoutBinding CONST_REFERENCE layout_binding);
        void set(VkDescriptorType type, uint32_t num_of_descriptor);

        operator VkDescriptorPool CONST_REFERENCE () const;

    protected:
        void release();

    private:
        VkDescriptorPoolCreateInfo _create_info = {};
        VkDevice _device = VK_NULL_HANDLE;

    };
};