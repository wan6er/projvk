#include "cvk/pipeline_compute.h"
#include "cvk/initialize/pipe_initialize.h"

namespace cvk
{
    
    ComputePipeline::ComputePipeline(VkDevice device) :
        BasePipeline(device)
    {
    }

    VkResult ComputePipeline::create(VkPipelineLayout layout, VkShaderModule shader)
    {
        set_stage(shader);
        __cvk::get_default_compute_pipeline_create_info(layout, *this, _create_info);
        return __cvk::create_computer_pipeline(device(), _create_info, object(), cache());
    }

} // namespace cvk
