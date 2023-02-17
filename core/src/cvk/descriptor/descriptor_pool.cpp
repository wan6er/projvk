#include "cvk/descriptor/descriptor_pool.h"
// #include "cvk/initialize/pipe_initialize.h"
#include "cvk/initialize/descriptor_initialize.h"
#include "utils/vector_util.h"



namespace cvk
{

DescriptorPool::DescriptorPool(VkDevice device) :
    _device(device)
{
    __cvk::get_default_descriptor_pool_create_info({}, 0, _create_info);
}

DescriptorPool::~DescriptorPool()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

VkResult DescriptorPool::create(uint32_t max_sets)
{
    CVK_ASSERT(max_sets > 0);
    __cvk::get_default_descriptor_pool_create_info(*this, max_sets, _create_info);
    return __cvk::create_descriptor_pool(_device, _create_info, object());
}

void DescriptorPool::set(VkDescriptorSetLayoutBinding CONST_REFERENCE layout_binding)
{
    CVK_ASSERT(object() == VK_NULL_HANDLE);
    VkDescriptorPoolSize pool_size = {};
    pool_size.type = layout_binding.descriptorType;
    pool_size.descriptorCount = layout_binding.descriptorCount;
    attaches(pool_size);
}

void DescriptorPool::set(VkDescriptorType type, uint32_t num_of_descriptor)
{
    CVK_ASSERT(object() == VK_NULL_HANDLE);
    VkDescriptorPoolSize pool_size = {};
    pool_size.type = type;
    pool_size.descriptorCount = num_of_descriptor;
    attaches(pool_size);
}

uint32_t& DescriptorPool::get(VkDescriptorType type)
{
    CVK_ASSERT(object() == VK_NULL_HANDLE);
    std::vector<VkDescriptorPoolSize>& pool_sizes = *this;
    for (auto& size : pool_sizes) {
        if (size.type == type) {
            return size.descriptorCount;
        }
    }

    VkDescriptorPoolSize pool_size = { type, 0 };
    return pool_sizes.emplace_back(pool_size).descriptorCount;
}

DescriptorPool::operator VkDescriptorPool CONST_REFERENCE () const
{
    return object();
}

VkDevice DescriptorPool::get_device() const
{
    return _device;
}

void DescriptorPool::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::destroy_descriptor_pool(_device, object());
    }
}

};