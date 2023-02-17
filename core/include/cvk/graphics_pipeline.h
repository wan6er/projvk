#pragma once

#include "cvk/vk_header.h"
#include "cvk/pipe/graphics_pipeline_deps.h"
#include "cvk/pipe/base_pipeline.h"
#include "utils/base_obj.h"
#include "utils/base_multi_attach.h"
#include "utils/base_single_attach.h"

namespace cvk
{

    class CVK_API GraphicsPipeline :
        public BasePipeline,
        public utils::BaseMultipleAttaches<VkPipelineShaderStageCreateInfo>,
        public utils::BaseSingleAttach<
            VertexInputState,
            InputAssemblyState,
            RasterizationState,
            ColorBlendState,
            DepthStencilState,
            ViewportState,
            MultisampleState,
            DynamicState,
            ShaderStage>
    {
    public:
        // explicit GraphicsPipeline();
        explicit GraphicsPipeline(VkDevice device, VkRenderPass renderpass, VkPipelineLayout layout);

        virtual VkResult create();

        void set_renderpass(VkRenderPass renderpass);
        void set_layout(VkPipelineLayout layout);
        void set_subpass(uint32_t subpass);

        template<class...__Args>
        void attaches(__Args&&...args);

        auto vertex_input() -> VertexInputState&;
        auto input_assembly() -> InputAssemblyState&;
        auto rasterization() -> RasterizationState&;
        auto color_blend() -> ColorBlendState&;
        auto depth_stencil() -> DepthStencilState&;
        auto viewport() -> ViewportState&;
        auto multisample() -> MultisampleState&;
        auto dynamic() -> DynamicState&;
        auto shader() -> ShaderStage&;

    protected:
        void setup_create_info();

    private:
        VkGraphicsPipelineCreateInfo _create_info;
    };

};

#include "graphics_pipeline.inl"