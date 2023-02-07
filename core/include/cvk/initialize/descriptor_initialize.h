#pragma once

#include "cvk/vk_header.h"

#include <vector>

namespace __cvk
{
    
    // descriptor set layout
    CVK_API void get_default_descriptor_set_layout_binding(VkDescriptorType type, VkShaderStageFlags shader_stage, uint32_t binding, uint32_t size, VkDescriptorSetLayoutBinding& layout_binding);
    CVK_API void get_default_descriptor_set_layout_create_info(std::vector<VkDescriptorSetLayoutBinding> CONST_REFERENCE bindings, VkDescriptorSetLayoutCreateInfo& create_info);
    CVK_API VkResult create_descriptor_set_layout(VkDevice device, VkDescriptorSetLayoutCreateInfo CONST_REFERENCE create_info, VkDescriptorSetLayout& dsl);
    CVK_API void destroy_descriptor_set_layout(VkDevice device, VkDescriptorSetLayout dsl);

    // descriptor set
    // CVK_API void get_default_descriptor_set_allocate_info(VkDescriptorPool pool, std::vector<VkDescriptorSetLayout> CONST_REFERENCE set_layout, VkDescriptorSetAllocateInfo& alloc_info);
    CVK_API void get_default_descriptor_buffer_info(uint32_t size, VkBuffer buffer, VkDescriptorBufferInfo& info, uint32_t offset = 0);
    CVK_API void get_default_descriptor_image_info(VkSampler sampler, VkImageView image_view, VkImageLayout layout, VkDescriptorImageInfo& info);
    CVK_API void get_default_write_descriptor_set_info(VkWriteDescriptorSet& info);
    CVK_API void get_write_descriptor_set_info(VkDescriptorSet set, VkDescriptorType type, uint32_t binding, VkWriteDescriptorSet& info);
    CVK_API void get_write_descriptor_set_info(std::vector<VkDescriptorImageInfo> CONST_REFERENCE images, std::vector<VkDescriptorBufferInfo> CONST_REFERENCE buffers, VkWriteDescriptorSet& info);
    CVK_API void get_default_descriptor_set_allocate_info(VkDescriptorPool pool, VkDescriptorSetLayout CONST_REFERENCE set_layout, VkDescriptorSetAllocateInfo& alloc_info);
    CVK_API void update_descriptor_set(VkDevice device, std::vector<VkWriteDescriptorSet> CONST_REFERENCE write_info);
    CVK_API VkResult alloc_descriptor_set(VkDevice device, VkDescriptorSetAllocateInfo CONST_REFERENCE alloc_info, VkDescriptorSet& set);
    CVK_API VkResult alloc_descriptor_sets(VkDevice device, VkDescriptorSetAllocateInfo CONST_REFERENCE alloc_info, std::vector<VkDescriptorSet>& sets);
    CVK_API void free_descriptor_set(VkDevice device, VkDescriptorPool pool, VkDescriptorSet set);

    // descriptor pool
    CVK_API void get_descriptor_pool_size_by_binding(VkDescriptorSetLayoutBinding CONST_REFERENCE binding, VkDescriptorPoolSize& pool_size);
    CVK_API void get_descriptor_pool_size(VkDescriptorType type, uint32_t count, VkDescriptorPoolSize& pool_size);
    CVK_API void get_default_descriptor_pool_create_info(std::vector<VkDescriptorPoolSize> CONST_REFERENCE pool_size, uint32_t max_size, VkDescriptorPoolCreateInfo& create_info);
    CVK_API VkResult create_descriptor_pool(VkDevice device, VkDescriptorPoolCreateInfo CONST_REFERENCE create_info, VkDescriptorPool& pool);
    CVK_API void destroy_descriptor_pool(VkDevice device, VkDescriptorPool pool);

}; 