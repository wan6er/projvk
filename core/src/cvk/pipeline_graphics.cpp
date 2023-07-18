#include "cvk/pipeline_graphics.h"
#include "cvk/initialize/pipe_initialize.h"



namespace cvk
{

// GraphicsPipeline::GraphicsPipeline()
// {
//     setup_create_info();
// }

GraphicsPipeline::GraphicsPipeline(VkDevice device) :
    BasePipeline(device)
{
    _create_info = {};
    setup_create_info();
    // _create_info.renderPass = renderpass;
    // _create_info.layout = layout;
}

// void GraphicsPipeline::set_renderpass(VkRenderPass renderpass)
// {
//     _create_info.renderPass = renderpass;
// }

// void GraphicsPipeline::set_layout(VkPipelineLayout layout)
// {
//     _create_info.layout = layout;
// }

// void GraphicsPipeline::set_subpass(uint32_t subpass)
// {
//     _create_info.subpass = subpass;
// }

VkResult GraphicsPipeline::create(VkRenderPass renderpass, VkPipelineLayout layout, uint32_t subpass)
{
    // setup_create_info();
    fill_other_info(renderpass, layout, subpass);
    return __cvk::create_graphics_pipeline(device(), _create_info, object());
}

void GraphicsPipeline::setup_create_info()
{
    _create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    _create_info.pVertexInputState = &static_cast<VkPipelineVertexInputStateCreateInfo CONST_REFERENCE>(*this);
    _create_info.pInputAssemblyState = &static_cast<VkPipelineInputAssemblyStateCreateInfo CONST_REFERENCE>(*this);
    _create_info.pViewportState = &static_cast<VkPipelineViewportStateCreateInfo CONST_REFERENCE>(*this);
    _create_info.pRasterizationState = &static_cast<VkPipelineRasterizationStateCreateInfo CONST_REFERENCE>(*this);
    _create_info.pMultisampleState = &static_cast<VkPipelineMultisampleStateCreateInfo CONST_REFERENCE>(*this);
    _create_info.pDepthStencilState = &static_cast<VkPipelineDepthStencilStateCreateInfo CONST_REFERENCE>(*this);
    _create_info.pColorBlendState = &static_cast<VkPipelineColorBlendStateCreateInfo CONST_REFERENCE>(*this);
    _create_info.pDynamicState = &static_cast<VkPipelineDynamicStateCreateInfo CONST_REFERENCE>(*this);
}

void GraphicsPipeline::fill_other_info(VkRenderPass renderpass, VkPipelineLayout layout, uint32_t subpass)
{
    std::vector<VkPipelineShaderStageCreateInfo> CONST_REFERENCE stages = shader();
    utils::vector_fill_info(stages, _create_info.stageCount, _create_info.pStages);
    _create_info.renderPass = renderpass;
    _create_info.layout = layout;
    _create_info.subpass = subpass;
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