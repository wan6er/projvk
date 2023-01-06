#pragma once

#include "descriptor_pool.h"
#include "descriptor_set_layout.h"

namespace cvk
{

    class Descriptor;

    class CVK_API DescriptorSetLayoutCallback :
        public DescriptorSetLayout
    {
    public:
        DescriptorSetLayoutCallback(DescriptorPool& pool, VkDevice device);

        virtual DescriptorSetLayout& set(uint32_t binding, VkDescriptorType type, VkShaderStageFlags shader_stage, uint32_t num_of_descriptor);

    private:

        DescriptorPool& _descriptor;
    };
};