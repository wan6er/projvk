#include "cvk/image/sampler.h"
#include "cvk/initialize/image_initialize.h"

namespace cvk
{

Sampler::Sampler(VkDevice device) :
    _device(device)
{
    __cvk::get_default_sampler_create_info(_create_info);
}

Sampler::~Sampler()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

VkResult Sampler::create()
{
    return __cvk::create_sampler(_device, _create_info, object());
}

Sampler::operator VkSampler CONST_REFERENCE () const
{
    return object();
}

void Sampler::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::destroy_sampler(_device, object());
    }
}

};