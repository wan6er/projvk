#include "cvk/pipe/descriptor_set.h"
#include "cvk/initialize/pipe_initialize.h"
#include "utils/vector_util.h"

namespace cvk
{

DescriptorSet::DescriptorSet(VkDevice device, VkDescriptorPool pool, VkDescriptorSetLayout CONST_REFERENCE layout) :
    _device(device)
{
    __cvk::get_default_descriptor_set_allocate_info(pool, layout, _alloc_info);
}

DescriptorSet::~DescriptorSet()
{
    if (isolated() && !is_cloned()) {
        free();
    }
}

DescriptorSet::operator VkDescriptorSet CONST_REFERENCE () const
{
    return object();
}

VkResult DescriptorSet::allocate()
{
    return __cvk::alloc_descriptor_set(_device, _alloc_info, object());
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