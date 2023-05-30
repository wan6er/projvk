#include "cvk/pipe/pipeline_deps_raytracing.h"

namespace cvk
{

void ShaderStages::add_stage(VkShaderStageFlagBits shader_stage, VkShaderModule shader)
{
    using BaseStages = utils::BaseMultipleAttachWrapper<VkPipelineShaderStageCreateInfo>;

    auto& stages = BaseStages::get_attachments();

    VkRayTracingShaderGroupCreateInfoKHR group_info {};
    group_info.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;

    group_info.generalShader = VK_SHADER_UNUSED_KHR;
    group_info.closestHitShader = VK_SHADER_UNUSED_KHR;
    group_info.anyHitShader = VK_SHADER_UNUSED_KHR;
    group_info.intersectionShader = VK_SHADER_UNUSED_KHR;

    switch (shader_stage) {
        case VK_SHADER_STAGE_RAYGEN_BIT_KHR:
        case VK_SHADER_STAGE_MISS_BIT_KHR:
            group_info.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
            group_info.generalShader = stages.size();
            break;
        case VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR:
            group_info.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
            group_info.closestHitShader = stages.size();
            break;
        case VK_SHADER_STAGE_ANY_HIT_BIT_KHR:
            group_info.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
            group_info.anyHitShader = stages.size();
            break;
        default:
            CVK_ASSERT(false);

    };
    attaches(group_info);    
    
    VkPipelineShaderStageCreateInfo stage_info {};
    stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stage_info.stage = shader_stage;
    stage_info.module = shader;
    stage_info.pName = "main";
    attaches(stage_info);
}

} // namespace cvk
