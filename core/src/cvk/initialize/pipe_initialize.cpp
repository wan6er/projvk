#include "cvk/initialize/pipe_initialize.h"
#include "utils/vector_util.h"



namespace __cvk
{

CVK_API void get_default_render_pass_create_info(std::vector<VkAttachmentDescription> CONST_REFERENCE attachments, std::vector<VkSubpassDescription> CONST_REFERENCE subpass, std::vector<VkSubpassDependency> CONST_REFERENCE dependencies, VkRenderPassCreateInfo& info) {
    info = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
    utils::vector_fill_info(attachments, info.attachmentCount, info.pAttachments);
    utils::vector_fill_info(subpass, info.subpassCount, info.pSubpasses);
    utils::vector_fill_info(dependencies, info.dependencyCount, info.pDependencies);
}

CVK_API VkResult create_render_pass(VkDevice device, VkRenderPassCreateInfo CONST_REFERENCE info, VkRenderPass& render_pass)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    return vkCreateRenderPass(device, &info, nullptr, &render_pass);
}

CVK_API void destroy_render_pass(VkDevice device, VkRenderPass render_pass)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(render_pass != VK_NULL_HANDLE);
    vkDestroyRenderPass(device, render_pass, nullptr);
}

CVK_API void get_default_attachment_description(VkFormat format, VkImageLayout final_layout, VkAttachmentDescription& description)
{
    description = {};
    description.flags = 0;
    description.format = format;
    description.samples = VK_SAMPLE_COUNT_1_BIT;
    description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    description.finalLayout = final_layout;
}

CVK_API void get_default_attachment_reference(uint32_t attachment, VkImageLayout attachment_layout, VkAttachmentReference& reference)
{
    reference = {};
    reference.attachment = attachment;
    reference.layout = attachment_layout;
}

CVK_API void get_default_subpass_description(VkPipelineBindPoint bind_point, std::vector<VkAttachmentReference> CONST_REFERENCE color, std::vector<VkAttachmentReference> CONST_REFERENCE input, VkAttachmentReference CONST_PTR depth, VkSubpassDescription& description)
{
    description = {};
    description.pipelineBindPoint = bind_point;
    description.pDepthStencilAttachment = depth;
    utils::vector_fill_info(input, description.inputAttachmentCount, description.pInputAttachments);
    utils::vector_fill_info(color, description.colorAttachmentCount, description.pColorAttachments);
}

CVK_API void get_default_subpass_dependency(uint32_t src, uint32_t dst, VkPipelineStageFlags src_stage, VkPipelineStageFlags dst_stage, VkAccessFlags src_access, VkAccessFlags dst_access, VkSubpassDependency& dependency)
{
    dependency = {};
    dependency.srcSubpass = src;
    dependency.dstSubpass = dst;
    dependency.srcStageMask = src_stage;
    dependency.dstStageMask = dst_stage;
    dependency.srcAccessMask = src_access;
    dependency.dstAccessMask = dst_access;
}

CVK_API void get_default_pipeline_layout_create_info(std::vector<VkDescriptorSetLayout> CONST_REFERENCE layouts, VkPipelineLayoutCreateInfo& create_info)
{
    create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    utils::vector_fill_info(layouts, create_info.setLayoutCount, create_info.pSetLayouts);
}

CVK_API VkResult create_pipeline_layout(VkDevice device, VkPipelineLayoutCreateInfo CONST_REFERENCE create_info, VkPipelineLayout& layout)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    return vkCreatePipelineLayout(device, &create_info, nullptr, &layout);
}

CVK_API void destroy_pipeline_layout(VkDevice device, VkPipelineLayout layout)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(layout != VK_NULL_HANDLE);
    vkDestroyPipelineLayout(device, layout, nullptr);
}

CVK_API void get_default_graphics_pipeline_create_info(VkGraphicsPipelineCreateInfo& create_info)
{
    create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
}

CVK_API VkResult create_graphics_pipeline(VkDevice device, VkGraphicsPipelineCreateInfo CONST_REFERENCE create_info, VkPipeline& pipeline, VkPipelineCache cache)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    return vkCreateGraphicsPipelines(device, cache, 1, &create_info, nullptr, &pipeline);
}
CVK_API void destroy_pipeline(VkDevice device, VkPipeline& pipeline)
{
    CVK_ASSERT(device != VK_NULL_HANDLE);
    CVK_ASSERT(pipeline != VK_NULL_HANDLE);
    vkDestroyPipeline(device, pipeline, nullptr);
}

};
