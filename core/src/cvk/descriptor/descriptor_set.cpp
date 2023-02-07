#include "cvk/descriptor/descriptor_set.h"
// #include "cvk/initialize/pipe_initialize.h"
#include "cvk/descriptor/write_descriptor_set.h"
#include "cvk/initialize/descriptor_initialize.h"
#include "utils/vector_util.h"

namespace cvk
{

DescriptorSet::DescriptorSet(VkDevice device, VkDescriptorPool pool) :
    DescriptorSetLayout(device),
    _device(device)
{
    __cvk::get_default_descriptor_set_allocate_info(pool, VK_NULL_HANDLE, _alloc_info);
}

DescriptorSet::DescriptorSet(VkDevice device) :
    DescriptorSetLayout(device),
    _device(device)
{
    __cvk::get_default_descriptor_set_allocate_info(VK_NULL_HANDLE, VK_NULL_HANDLE, _alloc_info);
}

DescriptorSet::DescriptorSet(VkDevice device, DescriptorSetLayout CONST_REFERENCE layout) :
    DescriptorSetLayout(layout),
    _device(device)
{
    __cvk::get_default_descriptor_set_allocate_info(VK_NULL_HANDLE, VK_NULL_HANDLE, _alloc_info);
}


DescriptorSet::~DescriptorSet()
{
    if (SetType::isolated() && !SetType::is_cloned()) {
        free();
    }
}

DescriptorSet::operator VkDescriptorSet CONST_REFERENCE () const
{
    return SetType::object();
}

VkResult DescriptorSet::create()
{
    CVK_ASSERT(_alloc_info.descriptorPool != VK_NULL_HANDLE);
    __cvk::get_default_descriptor_set_allocate_info(_alloc_info.descriptorPool, *this, _alloc_info);
    VkResult result = DescriptorSetLayout::create();
    if (result == VK_SUCCESS) {
        result = __cvk::alloc_descriptor_set(_device, _alloc_info, SetType::object());
    }
    return result;
}

VkResult DescriptorSet::create(VkDescriptorPool pool)
{
    __cvk::get_default_descriptor_set_allocate_info(pool, *this, _alloc_info);
    VkResult result = DescriptorSetLayout::create();
    if (result == VK_SUCCESS) {
        result = __cvk::alloc_descriptor_set(_device, _alloc_info, SetType::object());
    }
    return result;
}

void DescriptorSet::write(uint32_t binding, VkDescriptorImageInfo CONST_REFERENCE info)
{
    CVK_ASSERT((VkDescriptorSet)*this != VK_NULL_HANDLE);
    cvk::WriteDescriptorSet write_set;
    write_set.attaches(info);
    write_set.setup(*this, get_layout_binding(binding));
    write_set.update(_device);
}

void DescriptorSet::write(uint32_t binding, VkDescriptorBufferInfo CONST_REFERENCE info)
{
    CVK_ASSERT((VkDescriptorSet)*this != VK_NULL_HANDLE);
    cvk::WriteDescriptorSet write_set;
    write_set.attaches(info);
    write_set.setup(*this, get_layout_binding(binding));
    write_set.update(_device);
}

// void DescriptorSet::update()
// {
//     std::vector<VkWriteDescriptorSet> _temp_info;
//     std::vector<VkWriteDescriptorSet> CONST_REFERENCE origin_info = *this;
//     std::vector<WriteDescriptorSet> CONST_REFERENCE write_info = *this;
     
//     std::function origin_transitional = 
//         [&] (VkWriteDescriptorSet CONST_REFERENCE src, VkWriteDescriptorSet& dst) -> void {
//             dst = src;
//             dst.dstSet = object();
//         };
//     utils::vector_transition(origin_info, _temp_info, origin_transitional);
    
//     std::function transitional = 
//         [&] (WriteDescriptorSet CONST_REFERENCE src, VkWriteDescriptorSet& dst) -> void {
//             dst = src;
//             dst.dstSet = object();
//         };
//     utils::vector_transition(write_info, _temp_info, transitional);

//     __cvk::update_descriptor_set(_device, _temp_info);

// }

// void DescriptorSet::update()
// {
//     __cvk::update_descriptor_set(_device, *this);
// }

void DescriptorSet::free()
{
    // if (object() != VK_NULL_HANDLE) {
    //     __cvk::free_descriptor_set(_device, _alloc_info.descriptorPool, object());
    // }
}

};