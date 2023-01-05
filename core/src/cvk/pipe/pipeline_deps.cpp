#include "cvk/pipe/pipeline_deps.h"

namespace cvk
{

VertexInputState::VertexInputState() : VkPipelineVertexInputStateCreateInfo()
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
}

InputAssemblyState::InputAssemblyState() : VkPipelineInputAssemblyStateCreateInfo()
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
}

RasterizationState::RasterizationState() : VkPipelineRasterizationStateCreateInfo()
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    polygonMode = VK_POLYGON_MODE_FILL;
    cullMode = VK_CULL_MODE_NONE;
    frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    depthClampEnable = VK_FALSE;
    rasterizerDiscardEnable = VK_FALSE;
    depthBiasEnable = VK_FALSE;
    lineWidth = 1.0f;
}

ColorBlendState::ColorBlendState() : VkPipelineColorBlendStateCreateInfo()
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    attachmentCount = 0;
    pAttachments = nullptr;
}

ColorBlendState& ColorBlendState::attach(uint32_t color_mask, bool blend_enable)
{
    VkPipelineColorBlendAttachmentState blend_attachment = {};
    blend_attachment.colorWriteMask = color_mask;
    blend_attachment.blendEnable = blend_enable;
    attaches(blend_attachment);
    return *this;
}

DepthStencilState::DepthStencilState() : VkPipelineDepthStencilStateCreateInfo()
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthTestEnable = VK_TRUE;
    depthWriteEnable = VK_TRUE;
    depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthBoundsTestEnable = VK_FALSE;
    back.failOp = VK_STENCIL_OP_KEEP;
    back.passOp = VK_STENCIL_OP_KEEP;
    back.compareOp = VK_COMPARE_OP_ALWAYS;
    front = back;
    stencilTestEnable = VK_FALSE;
}

ViewportState::ViewportState() : VkPipelineViewportStateCreateInfo()
{
    memset(this, 0, sizeof(VkPipelineViewportStateCreateInfo));
    sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
}

void ViewportState::set_size(uint32_t viewport_count, uint32_t scissor_count)
{
    viewportCount = viewport_count;
    scissorCount = scissor_count;
}

MultisampleState::MultisampleState() : VkPipelineMultisampleStateCreateInfo()
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
}

DynamicState::DynamicState() : VkPipelineDynamicStateCreateInfo()
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
}

ShaderStage& ShaderStage::attach(VkShaderStageFlagBits shader_stage, VkShaderModule shader)
{
    VkPipelineShaderStageCreateInfo shader_stage_info = {};
    shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_info.stage = shader_stage;
    shader_stage_info.module = shader;
    shader_stage_info.pName = "main";
    attaches(shader_stage_info);
    return *this;
}

};