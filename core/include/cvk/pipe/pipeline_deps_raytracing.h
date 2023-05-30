#pragma once

#include "cvk/vk_header.h"
#include "utils/base_multi_attach.h"

namespace cvk
{

    struct CVK_API ShaderStages : 
        protected utils::BaseMultipleAttaches<VkPipelineShaderStageCreateInfo, VkRayTracingShaderGroupCreateInfoKHR>
    {
        void add_stage(VkShaderStageFlagBits shader_stage, VkShaderModule shader);

        inline auto& get_stages() const {
            using BaseStages = utils::BaseMultipleAttachWrapper<VkPipelineShaderStageCreateInfo>;
            return BaseStages::get_attachments();
        }

        inline auto& get_groups() const {
            using BaseGroups = utils::BaseMultipleAttachWrapper<VkRayTracingShaderGroupCreateInfoKHR>;
            return BaseGroups::get_attachments();
        }

    };

};
