#include "cvk/pipeline_raytracing.h"
#include "cvk/initialize/pipe_initialize.h"
#include "utils/vector_util.h"

namespace cvk
{

    RayTracingPipeline::RayTracingPipeline(VkDevice device) :
        BasePipeline(device)
    {
        __cvk::get_default_raytracing_pipeline_create_info(_create_info);
    }

    VkResult RayTracingPipeline::create(VkPipelineLayout layout)
    {
        __cvk::get_raytracing_pipeline_create_info(layout, _create_info);
        __cvk::get_raytracing_pipeline_create_info(_StagesWraper::get_attachments(), _create_info);
        __cvk::get_raytracing_pipeline_create_info(_GroupsWraper::get_attachments(), _create_info);

        return __cvk::create_raytracing_pipeline(device(), _create_info, object());
    }
    
} // namespace cvk
