#include "cvk/initialize/descriptor_initialize.h"
#include "utils/vector_util.h"

namespace __cvk
{

CVK_API void get_default_descriptor_set_layout_binding(VkDescriptorType type, VkShaderStageFlags shader_stage, uint32_t binding, uint32_t size, VkDescriptorSetLayoutBinding& layout_binding)
{
    layout_binding = {};
    layout_binding.binding = binding;
    layout_binding.descriptorType = type;
    layout_binding.descriptorCount = size;
    layout_binding.stageFlags = shader_stage;
    layout_binding.pImmutableSamplers = nullptr;
}

CVK_API void get_default_descriptor_set_layout_create_info(std::vector<VkDescriptorSetLayoutBinding> CONST_REFERENCE bindings, VkDescriptorSetLayoutCreateInfo& create_info)
{
    create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    utils::vector_fill_info(bindings, create_info.bindingCount, create_info.pBindings);
}

CVK_API VkResult create_descriptor_set_layout(VkDevice device, VkDescriptorSetLayoutCreateInfo CONST_REFERENCE create_info, VkDescriptorSetLayout& dsl)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    return vkCreateDescriptorSetLayout(device, &create_info, nullptr, &dsl);
}

CVK_API void destroy_descriptor_set_layout(VkDevice device, VkDescriptorSetLayout dsl)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    vkDestroyDescriptorSetLayout(device, dsl, nullptr);
}

CVK_API void get_default_descriptor_buffer_info(uint32_t size, VkBuffer buffer, VkDescriptorBufferInfo& info, uint32_t offset)
{
    info.offset = offset;
    info.range = size;
    info.buffer = buffer;
}

CVK_API void get_default_descriptor_image_info(VkSampler sampler, VkImageView image_view, VkImageLayout layout, VkDescriptorImageInfo& info)
{
    info.sampler = sampler;
    info.imageView = image_view;
    info.imageLayout = layout;
}

CVK_API void get_default_descriptor_set_allocate_info(VkDescriptorPool pool, VkDescriptorSetLayout CONST_REFERENCE set_layout, VkDescriptorSetAllocateInfo& alloc_info)
{
    alloc_info = {};
    
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = pool;
    alloc_info.descriptorSetCount = 1;
    alloc_info.pSetLayouts = &set_layout;
}

CVK_API void update_descriptor_set(VkDevice device, std::vector<VkWriteDescriptorSet> CONST_REFERENCE write_info)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    vkUpdateDescriptorSets(device, write_info.size(), write_info.data(), 0, nullptr);
}

CVK_API VkResult alloc_descriptor_set(VkDevice device, VkDescriptorSetAllocateInfo CONST_REFERENCE alloc_info, VkDescriptorSet& set)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    return vkAllocateDescriptorSets(device, &alloc_info, &set);
}

CVK_API VkResult alloc_descriptor_sets(VkDevice device, VkDescriptorSetAllocateInfo CONST_REFERENCE alloc_info, std::vector<VkDescriptorSet>& sets)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(sets.size() == alloc_info.descriptorSetCount);
    return vkAllocateDescriptorSets(device, &alloc_info, sets.data());
}

CVK_API void free_descriptor_set(VkDevice device, VkDescriptorPool pool, VkDescriptorSet set)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(pool != VK_NULL_HANDLE);
    CVK_ASSERT(set != VK_NULL_HANDLE);
    vkFreeDescriptorSets(device, pool, 1, &set);
}

CVK_API void get_descriptor_pool_size_by_binding(VkDescriptorSetLayoutBinding CONST_REFERENCE binding, VkDescriptorPoolSize& pool_size)
{
    pool_size = {};
    pool_size.type = binding.descriptorType;
    pool_size.descriptorCount = binding.descriptorCount;
}

CVK_API void get_descriptor_pool_size(VkDescriptorType type, uint32_t count, VkDescriptorPoolSize& pool_size)
{
    pool_size = {};
    pool_size.type = type;
    pool_size.descriptorCount = count;
}

CVK_API void get_default_descriptor_pool_create_info(std::vector<VkDescriptorPoolSize> CONST_REFERENCE pool_size, uint32_t max_size, VkDescriptorPoolCreateInfo& create_info)
{
    create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    create_info.maxSets = max_size;
    utils::vector_fill_info(pool_size, create_info.poolSizeCount, create_info.pPoolSizes);
}

CVK_API VkResult create_descriptor_pool(VkDevice device, VkDescriptorPoolCreateInfo CONST_REFERENCE create_info, VkDescriptorPool& pool) 
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    return vkCreateDescriptorPool(device, &create_info, nullptr, &pool);
}

CVK_API void destroy_descriptor_pool(VkDevice device, VkDescriptorPool pool)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(pool != VK_NULL_HANDLE);
    vkDestroyDescriptorPool(device, pool, nullptr);
}

};
