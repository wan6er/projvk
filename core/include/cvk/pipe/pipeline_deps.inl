#pragma once

#include "utils/vector_util.h"

namespace cvk
{

template<class...__Args>
void VertexInputState::attaches(__Args&&...args)
{
    utils::BaseMultipleAttaches<VkVertexInputBindingDescription, VkVertexInputAttributeDescription>::attaches(std::forward<__Args>(args)...);
    
    std::vector<VkVertexInputBindingDescription> CONST_REFERENCE bindings = *this;
    utils::vector_fill_info(bindings, vertexBindingDescriptionCount, pVertexBindingDescriptions);
    std::vector<VkVertexInputAttributeDescription> CONST_REFERENCE attributes = *this;
    utils::vector_fill_info(attributes, vertexAttributeDescriptionCount, pVertexAttributeDescriptions);
}

template<class...__Args>
void ColorBlendState::attaches(__Args&&...args)
{
    utils::BaseMultipleAttaches<VkPipelineColorBlendAttachmentState>::attaches(std::forward<__Args>(args)...);

    std::vector<VkPipelineColorBlendAttachmentState> CONST_REFERENCE color_blend_states = *this;
    utils::vector_fill_info(color_blend_states, attachmentCount, pAttachments);
}

template<class...__Args>
void ViewportState::attaches(__Args&&...args)
{
    utils::BaseMultipleAttaches<VkViewport, VkRect2D>::attaches(std::forward<__Args>(args)...);

    std::vector<VkViewport> CONST_REFERENCE viewports = *this;
    utils::vector_fill_info(viewports, viewportCount, pViewports);
    std::vector<VkRect2D> CONST_REFERENCE scissors = *this;
    utils::vector_fill_info(scissors, scissorCount, pScissors);
}

template<class...__Args>
void DynamicState::attaches(__Args&&...args)
{
    utils::BaseMultipleAttaches<VkDynamicState>::attaches(std::forward<__Args>(args)...);

    std::vector<VkDynamicState> CONST_REFERENCE dynamic_states = *this;
    utils::vector_fill_info(dynamic_states, dynamicStateCount, pDynamicStates);
}

};