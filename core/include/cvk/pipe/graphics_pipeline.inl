#pragma once

#include "utils/vector_util.h"

namespace cvk
{

template<class...__Args>
void GraphicsPipeline::attaches(__Args&&...args)
{
    utils::BaseMultipleAttaches<VkPipelineShaderStageCreateInfo>::attaches(std::forward<__Args>(args)...);

    std::vector<VkPipelineShaderStageCreateInfo> CONST_REFERENCE stage_create_infos = *this;
    utils::vector_fill_info(stage_create_infos, _create_info.stageCount, _create_info.pStages);
}


};