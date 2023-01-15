#pragma once

#include "cvk/vk_header.h"

#include <vector>

namespace __cvk
{
    CVK_API void get_default_render_pass_create_info(std::vector<VkAttachmentDescription> CONST_REFERENCE attachments, std::vector<VkSubpassDescription> CONST_REFERENCE subpass, std::vector<VkSubpassDependency> CONST_REFERENCE dependencies, VkRenderPassCreateInfo& info);
    CVK_API VkResult create_render_pass(VkDevice device, VkRenderPassCreateInfo CONST_REFERENCE info, VkRenderPass& render_pass);
    CVK_API void destroy_render_pass(VkDevice device, VkRenderPass render_pass);

    CVK_API void get_default_attachment_description(VkFormat format, VkImageLayout final_layout, VkAttachmentDescription& description);
    
    // subpass
    CVK_API void get_default_attachment_reference(uint32_t attachment, VkImageLayout attachment_layout, VkAttachmentReference& reference);
    CVK_API void get_default_subpass_description(VkPipelineBindPoint bind_point, std::vector<VkAttachmentReference> CONST_REFERENCE color, std::vector<VkAttachmentReference> CONST_REFERENCE input, VkAttachmentReference CONST_PTR depth, VkSubpassDescription& description);
    CVK_API void get_default_subpass_dependency(uint32_t src, uint32_t dst, VkPipelineStageFlags src_stage, VkPipelineStageFlags dst_stage, VkAccessFlags src_access, VkAccessFlags dst_access, VkSubpassDependency& dependency);

    // descriptor set layout
    CVK_API void get_default_descriptor_set_layout_binding(VkDescriptorType type, VkShaderStageFlags shader_stage, uint32_t binding, uint32_t size, VkDescriptorSetLayoutBinding& layout_binding);
    CVK_API void get_default_descriptor_set_layout_create_info(std::vector<VkDescriptorSetLayoutBinding> CONST_REFERENCE bindings, VkDescriptorSetLayoutCreateInfo& create_info);
    CVK_API VkResult create_descriptor_set_layout(VkDevice device, VkDescriptorSetLayoutCreateInfo CONST_REFERENCE create_info, VkDescriptorSetLayout& dsl);
    CVK_API void destroy_descriptor_set_layout(VkDevice device, VkDescriptorSetLayout dsl);

    // descriptor set
    // CVK_API void get_default_descriptor_set_allocate_info(VkDescriptorPool pool, std::vector<VkDescriptorSetLayout> CONST_REFERENCE set_layout, VkDescriptorSetAllocateInfo& alloc_info);
    CVK_API void get_default_descriptor_buffer_info(uint32_t size, VkBuffer buffer, VkDescriptorBufferInfo& info, uint32_t offset = 0);
    CVK_API void get_default_descriptor_image_info(VkSampler sampler, VkImageView image_view, VkImageLayout layout, VkDescriptorImageInfo& info);
    CVK_API void get_default_descriptor_set_allocate_info(VkDescriptorPool pool, VkDescriptorSetLayout CONST_REFERENCE set_layout, VkDescriptorSetAllocateInfo& alloc_info);
    CVK_API void update_descriptor_set(VkDevice device, std::vector<VkWriteDescriptorSet> CONST_REFERENCE write_info);
    CVK_API VkResult alloc_descriptor_set(VkDevice device, VkDescriptorSetAllocateInfo CONST_REFERENCE alloc_info, VkDescriptorSet& set);
    CVK_API void free_descriptor_set(VkDevice device, VkDescriptorPool pool, VkDescriptorSet set);

    // descriptor pool
    CVK_API void get_descriptor_pool_size_by_binding(VkDescriptorSetLayoutBinding CONST_REFERENCE binding, VkDescriptorPoolSize& pool_size);
    CVK_API void get_descriptor_pool_size(VkDescriptorType type, uint32_t count, VkDescriptorPoolSize& pool_size);
    CVK_API void get_default_descriptor_pool_create_info(std::vector<VkDescriptorPoolSize> CONST_REFERENCE pool_size, uint32_t max_size, VkDescriptorPoolCreateInfo& create_info);
    CVK_API VkResult create_descriptor_pool(VkDevice device, VkDescriptorPoolCreateInfo CONST_REFERENCE create_info, VkDescriptorPool& pool);
    CVK_API void destroy_descriptor_pool(VkDevice device, VkDescriptorPool pool);

    // pipeline layout
    CVK_API void get_default_pipeline_layout_create_info(std::vector<VkDescriptorSetLayout> CONST_REFERENCE layouts, VkPipelineLayoutCreateInfo& create_info);
    CVK_API VkResult create_pipeline_layout(VkDevice device, VkPipelineLayoutCreateInfo CONST_REFERENCE create_info, VkPipelineLayout& layout);
    CVK_API void destroy_pipeline_layout(VkDevice device, VkPipelineLayout layout);

    // graphics pipeline
    CVK_API void get_default_graphics_pipeline_create_info(VkGraphicsPipelineCreateInfo& create_info);
    CVK_API VkResult create_graphics_pipeline(VkDevice device, VkGraphicsPipelineCreateInfo CONST_REFERENCE create_info, VkPipeline& pipeline, VkPipelineCache cache = VK_NULL_HANDLE);
    
    // pipline
    CVK_API void destroy_pipeline(VkDevice device, VkPipeline& pipeline);
}; 