#include "cvk/descriptor/write_descriptor_set.h"
// #include "cvk/initialize/pipe_initialize.h"
#include "cvk/initialize/descriptor_initialize.h"

namespace cvk
{

WriteDescriptorSet::WriteDescriptorSet()
{
    __cvk::get_default_write_descriptor_set_info(_write);
}

auto WriteDescriptorSet::setup(VkDescriptorSet set, VkDescriptorType type, uint32_t binding) -> WriteDescriptorSet&
{
    __cvk::get_write_descriptor_set_info(set, type, binding, _write);
    // return __cvk::update_descriptor_set(_device, { _write });
    return *this;
}

auto WriteDescriptorSet::setup(VkDescriptorSet set, VkDescriptorSetLayoutBinding CONST_REFERENCE layout_binding) -> WriteDescriptorSet&
{
    this->setup(set, layout_binding.descriptorType, layout_binding.binding);
    return *this;
}

void WriteDescriptorSet::update(VkDevice device) const
{
    __cvk::update_descriptor_set(device, { _write });
}

WriteDescriptorSet::operator VkWriteDescriptorSet CONST_REFERENCE () const
{
    return _write;
}

};