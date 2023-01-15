#include "cvk/pipe/pipeline_layout.h"
#include "cvk/initialize/pipe_initialize.h"

namespace cvk
{

PipelineLayout::PipelineLayout(VkDevice device) :
    _device(device)
{
    __cvk::get_default_pipeline_layout_create_info({}, _create_info);
}

PipelineLayout::~PipelineLayout()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

VkResult PipelineLayout::create()
{
    __cvk::get_default_pipeline_layout_create_info(*this, _create_info);
    return __cvk::create_pipeline_layout(_device, _create_info, object());
}

PipelineLayout::operator VkPipelineLayout CONST_REFERENCE () const
{
    return object();
}

void PipelineLayout::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::destroy_pipeline_layout(_device, object());
    }
}

};