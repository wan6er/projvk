#pragma once

#include "cvk/vk_header.h"
#include "cvk/descriptor/descriptor_pool.h"
#include "cvk/descriptor/descriptor_set_layout_callback.h"
#include "cvk/descriptor/descriptor_set.h"
#include "utils/base_multi_attach.h"

namespace cvk
{
    
    class CVK_API Descriptor :
        public DescriptorPool,
        public utils::BaseMultipleAttaches<DescriptorSetLayoutCallback, DescriptorSet>
    {
    public:
        Descriptor(VkDevice device);
        virtual ~Descriptor();

        DescriptorSetLayout& add_layout();
        DescriptorSetLayout& get_layout(size_t index);
        VkResult create();

        DescriptorSet& operator[](size_t set);
        DescriptorSet CONST_REFERENCE operator[](size_t set) const;

    protected:
        VkResult create_pool();
        VkResult create_sets();
    };

} // namespace cvk
