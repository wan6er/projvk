#pragma once

#include "descriptor_pool.h"
#include "descriptor_set.h"

namespace cvk
{

    class Descriptor;

    class CVK_API DescriptorSetLayoutCallback :
        public DescriptorSet
    {
    public:
        DescriptorSetLayoutCallback(DescriptorPool& pool, VkDevice device);
        DescriptorSetLayoutCallback& operator=(DescriptorSetLayoutCallback CONST_REFERENCE callback);

        virtual DescriptorSetLayout& set(uint32_t binding, VkDescriptorType type, VkShaderStageFlags shader_stage, uint32_t num_of_descriptor);

    private:

        DescriptorPool& _descriptor;
    };
};