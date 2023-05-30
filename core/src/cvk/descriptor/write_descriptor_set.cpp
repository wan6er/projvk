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
    using _BufferInfos = utils::BaseMultipleAttachWrapper<VkDescriptorBufferInfo>;
    using _ImageInfos = utils::BaseMultipleAttachWrapper<VkDescriptorImageInfo>;
    using _AccStructInfos = utils::BaseMultipleAttachWrapper<VkAccelerationStructureKHR>;

    size_t desc_size = 0;

    auto fill_info = [&] (auto& array, auto& info, void* prev = nullptr) {
        if (array.size() > 0) {
            __cvk::get_write_descriptor_set_info(array, info, prev);
            desc_size += array.size();
        }
    };

    fill_info(_BufferInfos::get_attachments(), _write);
    fill_info(_ImageInfos::get_attachments(), _write);
    fill_info(_AccStructInfos::get_attachments(), _write_accstruct, &_write);

    __cvk::get_write_descriptor_set_info(set, type, binding, desc_size, _write);
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