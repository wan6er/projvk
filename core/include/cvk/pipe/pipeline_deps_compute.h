#pragma once

#include "cvk/vk_header.h"
#include "utils/base_single_attach.h"

namespace cvk
{

    struct CVK_API ComputeShaderStage : 
        protected utils::BaseSingleAttach<VkPipelineShaderStageCreateInfo>
    {
        void set_stage(VkShaderModule shader);

    };

};
