#include "cvk/pipe/pipeline_deps_compute.h"

namespace cvk
{

    void ComputeShaderStage::set_stage(VkShaderModule shader)
    {
        VkPipelineShaderStageCreateInfo shader_stage_info {};
        shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stage_info.stage = VK_SHADER_STAGE_COMPUTE_BIT;
        shader_stage_info.module = shader;
        shader_stage_info.pName = "main";
        attach(shader_stage_info);
    }
    
} // namespace cvk
