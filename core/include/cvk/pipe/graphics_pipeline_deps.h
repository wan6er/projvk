#pragma once

#include "cvk/vk_header.h"
#include "utils/base_multi_attach.h"

namespace cvk
{
    struct VertexInputState;

    struct CVK_API VertexInputAttributes
    {
        VertexInputAttributes(VertexInputState& state, uint32_t binding);

        VertexInputAttributes& add_attribute(uint32_t location, VkFormat format, uint32_t offset);

        VertexInputAttributes& operator=(VertexInputAttributes CONST_REFERENCE);

        VertexInputState& vertex_input;
        uint32_t binding;

    };

    struct CVK_API VertexInputState : 
        public VkPipelineVertexInputStateCreateInfo,
        public utils::BaseMultipleAttaches<VkVertexInputBindingDescription, VkVertexInputAttributeDescription, VertexInputAttributes>
    {
        VertexInputState();

        VertexInputAttributes& add_binding(uint32_t binding, uint32_t stride);
        VertexInputAttributes& get_binding(uint32_t binding);

        template<class...__Args>
        void attaches(__Args&&...args);

    };

    struct CVK_API InputAssemblyState : 
        public VkPipelineInputAssemblyStateCreateInfo
    {
        InputAssemblyState();
    };    
    
    struct CVK_API RasterizationState : 
        public VkPipelineRasterizationStateCreateInfo
    {
        RasterizationState();
    };

    struct CVK_API ColorBlendState : 
        public VkPipelineColorBlendStateCreateInfo,
        public utils::BaseMultipleAttaches<VkPipelineColorBlendAttachmentState>
    {
        ColorBlendState();

        ColorBlendState& attach(uint32_t color_mask, bool blend_enable);

        template<class...__Args>
        void attaches(__Args&&...args);
    };
    
    struct CVK_API DepthStencilState : 
        public VkPipelineDepthStencilStateCreateInfo
    {
        DepthStencilState();
    };

    struct CVK_API ViewportState : 
        public VkPipelineViewportStateCreateInfo,
        public utils::BaseMultipleAttaches<VkViewport, VkRect2D>
    {
        ViewportState();

        void set_size(uint32_t viewport_count, uint32_t scissor_count);

        template<class...__Args>
        void attaches(__Args&&...args);
    };
    
    struct CVK_API MultisampleState : 
        public VkPipelineMultisampleStateCreateInfo
    {
        MultisampleState();
    };

    struct CVK_API DynamicState : 
        public VkPipelineDynamicStateCreateInfo,
        public utils::BaseMultipleAttaches<VkDynamicState>
    {
        DynamicState();
        
        template<class...__Args>
        void attaches(__Args&&...args);
    };

    struct CVK_API ShaderStage : 
        public utils::BaseMultipleAttaches<VkPipelineShaderStageCreateInfo>
    {
        ShaderStage& attach(VkShaderStageFlagBits shader_stage, VkShaderModule shader);
    };
};

#include "graphics_pipeline_deps.inl"