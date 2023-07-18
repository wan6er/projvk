#pragma once

#include "cvk/vk_header.h"
#include "cvk/pipe/pipeline_layout.h"
#include "cvk/pipe/pipeline_deps_graphics.h"
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
        explicit GraphicsPipeline(VkDevice device);

        virtual VkResult create(VkRenderPass renderpass, VkPipelineLayout layout, uint32_t subpass = 0);

        // void set_renderpass(VkRenderPass renderpass);
        // void set_layout(VkPipelineLayout layout);
        // void set_subpass(uint32_t subpass);

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
        void fill_other_info(VkRenderPass renderpass, VkPipelineLayout layout, uint32_t subpass);

    private:
        VkGraphicsPipelineCreateInfo _create_info;
    };

};

#include "pipeline_graphics.inl"