#pragma once

#include "cvk/vk_header.h"
#include "cvk/descriptor/descriptor_pool.h"
#include "cvk/descriptor/descriptor_set_layout_callback.h"
#include "cvk/descriptor/descriptor_set.h"
#include "cvk/descriptor/write_descriptor_set.h"
#include "utils/base_multi_attach.h"

namespace cvk
{
    
    class CVK_API Descriptor :
        public DescriptorPool,
        public utils::BaseMultipleAttaches<DescriptorSetLayoutCallback>
    {
    public:
        Descriptor(VkDevice device);
        virtual ~Descriptor();

        DescriptorSetLayout& add_layout();
        void add_layout(DescriptorSetLayout CONST_REFERENCE layout);
        VkResult create();

        DescriptorSetLayoutCallback& operator[](size_t set);
        DescriptorSetLayoutCallback CONST_REFERENCE operator[](size_t set) const;

    protected:
        using DescriptorPool::create;
        
        VkResult create_pool();
        VkResult create_sets();
    };

} // namespace cvk
