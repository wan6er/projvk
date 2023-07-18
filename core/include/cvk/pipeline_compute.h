#pragma once

#include "cvk/vk_header.h"
#include "cvk/pipe/base_pipeline.h"
#include "cvk/pipe/pipeline_deps_compute.h"
#include "cvk/pipe/pipeline_layout.h"

namespace cvk
{

    class CVK_API ComputePipeline : 
        public BasePipeline, protected ComputeShaderStage
    {
    public:
        ComputePipeline(VkDevice device);

        virtual VkResult create(VkPipelineLayout layout, VkShaderModule shader);

    private:
        VkComputePipelineCreateInfo _create_info {};

    };

};