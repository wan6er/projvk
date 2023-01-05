#include "cvk/pipe/graphics_pipeline.h"
#include "cvk/initialize/pipe_initialize.h"



namespace cvk
{

// GraphicsPipeline::GraphicsPipeline()
// {
//     setup_create_info();
// }

GraphicsPipeline::GraphicsPipeline(VkRenderPass renderpass, VkPipelineLayout layout)
{
    setup_create_info();
    _create_info.renderPass = renderpass;
    _create_info.layout = layout;
}

void GraphicsPipeline::set_renderpass(VkRenderPass renderpass)
{
    _create_info.renderPass = renderpass;
}

void GraphicsPipeline::set_layout(VkPipelineLayout layout)
{
    _create_info.layout = layout;
}

VkResult GraphicsPipeline::create(VkDevice dev)
{
    CVK_ASSERT(_create_info.renderPass != VK_NULL_HANDLE);
    CVK_ASSERT(_create_info.layout != VK_NULL_HANDLE);
    // VkGraphicsPipelineCreateInfo;
    device = dev;
    std::vector<VkPipelineShaderStageCreateInfo> CONST_REFERENCE stages = shader();
    utils::vector_fill_info(stages, _create_info.stageCount, _create_info.pStages);
    return __cvk::create_graphics_pipeline(device, _create_info, object());
}

void GraphicsPipeline::setup_create_info()
{
    _create_info = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pVertexInputState = &static_cast<VkPipelineVertexInputStateCreateInfo CONST_REFERENCE>(*this),
        .pInputAssemblyState = &static_cast<VkPipelineInputAssemblyStateCreateInfo CONST_REFERENCE>(*this),
        .pViewportState = &static_cast<VkPipelineViewportStateCreateInfo CONST_REFERENCE>(*this),
        .pRasterizationState = &static_cast<VkPipelineRasterizationStateCreateInfo CONST_REFERENCE>(*this),
        .pMultisampleState = &static_cast<VkPipelineMultisampleStateCreateInfo CONST_REFERENCE>(*this),
        .pDepthStencilState = &static_cast<VkPipelineDepthStencilStateCreateInfo CONST_REFERENCE>(*this),
        .pColorBlendState = &static_cast<VkPipelineColorBlendStateCreateInfo CONST_REFERENCE>(*this),
        .pDynamicState = &static_cast<VkPipelineDynamicStateCreateInfo CONST_REFERENCE>(*this),
    };
}

auto GraphicsPipeline::vertex_input() -> VertexInputState&
{
    return *this;
}
auto GraphicsPipeline::input_assembly() -> InputAssemblyState&
{
    return *this;
}
auto GraphicsPipeline::rasterization() -> RasterizationState&
{
    return *this;
}
auto GraphicsPipeline::color_blend() -> ColorBlendState&
{
    return *this;
}
auto GraphicsPipeline::depth_stencil() -> DepthStencilState&
{
    return *this;
}
auto GraphicsPipeline::viewport() -> ViewportState&
{
    return *this;
}
auto GraphicsPipeline::multisample() -> MultisampleState&
{
    return *this;
}
auto GraphicsPipeline::dynamic() -> DynamicState&
{
    return *this;
}
auto GraphicsPipeline::shader() -> ShaderStage&
{
    return *this;
}

};