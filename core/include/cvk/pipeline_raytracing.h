#pragma once

#include "cvk/vk_header.h"
#include "cvk/pipe/base_pipeline.h"
#include "cvk/pipe/pipeline_layout.h"
#include "cvk/pipe/pipeline_deps_graphics.h"
#include "cvk/pipe/pipeline_deps_raytracing.h"
#include "utils/base_single_attach.h"

namespace cvk
{

    class CVK_API RayTracingPipeline : 
        public BasePipeline, public ShaderStages
    {
        using _StagesWraper = utils::BaseMultipleAttachWrapper<VkPipelineShaderStageCreateInfo>;
        using _GroupsWraper = utils::BaseMultipleAttachWrapper<VkRayTracingShaderGroupCreateInfoKHR>;

    public:
        RayTracingPipeline(VkDevice device);

        virtual VkResult create(VkPipelineLayout layout);

    private:
        VkRayTracingPipelineCreateInfoKHR _create_info {};

    };

};

// #include "pipeline_graphics.inl"
