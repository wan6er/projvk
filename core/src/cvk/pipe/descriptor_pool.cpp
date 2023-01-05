#include "cvk/pipe/descriptor_pool.h"
#include "cvk/initialize/pipe_initialize.h"
#include "utils/vector_util.h"



namespace cvk
{

DescriptorPool::DescriptorPool(uint32_t max_sets)
{
    __cvk::get_default_descriptor_pool_create_info({}, max_sets, _create_info);
}

DescriptorPool::~DescriptorPool()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

VkResult DescriptorPool::create(VkDevice device)
{
    _device = device;
    // std::vector<VkDescriptorPoolSize> temp_pool_size = *this;
    // std::vector<VkDescriptorSetLayoutBinding> CONST_REFERENCE bindings = *this;
    // std::function transitional = __cvk::get_descriptor_pool_size_by_binding;
    // utils::vector_transition(bindings, temp_pool_size, transitional);

    __cvk::get_default_descriptor_pool_create_info(*this, _create_info.maxSets, _create_info);
    return __cvk::create_descriptor_pool(_device, _create_info, object());
}

void DescriptorPool::set(VkDescriptorSetLayoutBinding CONST_REFERENCE layout_binding)
{
    CVK_ASSERT(object() == VK_NULL_HANDLE);
    VkDescriptorPoolSize pool_size = { .type = layout_binding.descriptorType, .descriptorCount = layout_binding.descriptorCount };
    attaches(pool_size);
}

void DescriptorPool::set(VkDescriptorType type, uint32_t num_of_descriptor)
{
    CVK_ASSERT(object() == VK_NULL_HANDLE);
    VkDescriptorPoolSize pool_size = { .type = type, .descriptorCount = num_of_descriptor };
    attaches(pool_size);
}

DescriptorPool::operator VkDescriptorPool CONST_REFERENCE () const
{
    return object();
}

void DescriptorPool::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::destroy_descriptor_pool(_device, object());
    }
}

};