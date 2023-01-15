#include "cvk/descriptor/write_descriptor_set.h"
#include "cvk/initialize/pipe_initialize.h"

namespace cvk
{

// WriteDescriptorSet::WriteDescriptorSet()
// {
//     _write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
// }

WriteDescriptorSet::WriteDescriptorSet(VkDescriptorType type, uint32_t binding)
{
    _write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    _write.descriptorType = type;
    _write.dstBinding = binding;
}

WriteDescriptorSet::WriteDescriptorSet(VkDescriptorSetLayoutBinding CONST_REFERENCE layout_binding)
{
    _write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    _write.descriptorType = layout_binding.descriptorType;
    _write.dstBinding = layout_binding.binding;
}

void WriteDescriptorSet::update(VkDevice device, VkDescriptorSet set)
{
    _write.dstSet = set;
    return __cvk::update_descriptor_set(device, { _write });
}

WriteDescriptorSet::operator VkWriteDescriptorSet CONST_REFERENCE () const
{
    return _write;
}

};