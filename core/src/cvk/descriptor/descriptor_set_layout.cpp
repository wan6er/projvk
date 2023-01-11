#include "cvk/descriptor/descriptor_set_layout.h"
#include "cvk/initialize/pipe_initialize.h"

namespace cvk
{


DescriptorSetLayoutBinding::DescriptorSetLayoutBinding(VkDescriptorType type, VkShaderStageFlags shader_stage, uint32_t binding, uint32_t num_of_descriptor)
{
    __cvk::get_default_descriptor_set_layout_binding(type, shader_stage, binding, num_of_descriptor, *this);
}

// VkDescriptorPoolSize DescriptorSetLayoutBinding::get_pool_size() const
// {
//     VkDescriptorPoolSize pool_size = {};
//     pool_size.type = descriptorType;
//     pool_size.descriptorCount = descriptorCount;
//     return pool_size;
// }

DescriptorSetLayout::DescriptorSetLayout(VkDevice device) :
    // _descriptor(pool),
    _device(device)
{
    __cvk::get_default_descriptor_set_layout_create_info({}, _create_info);
}

DescriptorSetLayout::~DescriptorSetLayout()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

DescriptorSetLayout& DescriptorSetLayout::set(uint32_t binding, VkDescriptorType type, VkShaderStageFlags shader_stage, uint32_t num_of_descriptor)
{
    // _descriptor.get(type)++;

    VkDescriptorSetLayoutBinding layout_binding;
    __cvk::get_default_descriptor_set_layout_binding(type, shader_stage, binding, num_of_descriptor, layout_binding);
    attaches(layout_binding);
    return *this;
}

DescriptorSetLayout::operator VkDescriptorSetLayout CONST_REFERENCE () const
{
    return object();
}

VkDescriptorSetLayout CONST_REFERENCE DescriptorSetLayout::get_layout() const
{
    return object();
}

VkResult DescriptorSetLayout::create()
{
    // _temp_bindings = *this;
    // std::vector<DescriptorSetLayoutBinding> src_bindings = *this;
    // if (src_bindings.size() > 0) {
    //     _temp_bindings.insert(_temp_bindings.end(), src_bindings.begin(), src_bindings.end());
    // }
    __cvk::get_default_descriptor_set_layout_create_info(*this, _create_info);
    return __cvk::create_descriptor_set_layout(_device, _create_info, object());
}

void DescriptorSetLayout::release()
{
    __cvk::destroy_descriptor_set_layout(_device, object());
}

};