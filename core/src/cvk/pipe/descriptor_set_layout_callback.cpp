#include "cvk/pipe/descriptor_set_layout_callback.h"

namespace cvk
{
    
DescriptorSetLayoutCallback::DescriptorSetLayoutCallback(DescriptorPool& pool, VkDevice device) :
    DescriptorSetLayout(device),
    _descriptor(pool)
{
}

DescriptorSetLayout& DescriptorSetLayoutCallback::set(uint32_t binding, VkDescriptorType type, VkShaderStageFlags shader_stage, uint32_t num_of_descriptor)
{
    _descriptor.get(type)++;
    return DescriptorSetLayout::set(binding, type, shader_stage, num_of_descriptor);
}


} // namespace cvk
