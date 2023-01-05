#include "cvk/pipe/base_pipeline.h"
#include "cvk/initialize/pipe_initialize.h"

namespace cvk
{

BasePipeline::BasePipeline()
{
}

BasePipeline::~BasePipeline()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

BasePipeline::operator VkPipeline() const
{
    return object();
}

void BasePipeline::release()
{
    if (device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::destroy_pipeline(device, object());
    }
}

};
