#include "cvk/descriptor/descriptor_set_layout_callback.h"

namespace cvk
{
    
DescriptorSetLayoutCallback::DescriptorSetLayoutCallback(DescriptorPool& pool, VkDevice device) :
    DescriptorSet(device),
    _descriptor(pool)
{
}

DescriptorSetLayoutCallback::DescriptorSetLayoutCallback(DescriptorPool& pool, VkDevice device, DescriptorSetLayout CONST_REFERENCE layout) :
    DescriptorSet(device, layout),
    _descriptor(pool)
{
}
// DescriptorSetLayoutCallback& DescriptorSetLayoutCallback::operator=(DescriptorSetLayoutCallback CONST_REFERENCE callback)
// {
//     _descriptor = callback._descriptor;
//     return *this;
// }

DescriptorSetLayout& DescriptorSetLayoutCallback::set(uint32_t binding, VkDescriptorType type, VkShaderStageFlags shader_stage, uint32_t num_of_descriptor)
{
    _descriptor.get(type)++;
    return DescriptorSet::set(binding, type, shader_stage, num_of_descriptor);
}


} // namespace cvk
