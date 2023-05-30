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

namespace __cvk
{

    CVK_API void get_default_raytracing_pipeline_create_info(VkRayTracingPipelineCreateInfoKHR& create_info);
    CVK_API void get_raytracing_pipeline_create_info(VkPipelineLayout layout, VkRayTracingPipelineCreateInfoKHR& create_info);
    CVK_API void get_raytracing_pipeline_create_info(std::vector<VkPipelineShaderStageCreateInfo> CONST_REFERENCE stages, VkRayTracingPipelineCreateInfoKHR& create_info);
    CVK_API void get_raytracing_pipeline_create_info(std::vector<VkRayTracingShaderGroupCreateInfoKHR> CONST_REFERENCE groups, VkRayTracingPipelineCreateInfoKHR& create_info);
    // raytracing pipeline
    CVK_API VkResult create_raytracing_pipeline(VkDevice device, VkRayTracingPipelineCreateInfoKHR CONST_REFERENCE info, VkPipeline& pipeline, VkPipelineCache cache = VK_NULL_HANDLE);

}