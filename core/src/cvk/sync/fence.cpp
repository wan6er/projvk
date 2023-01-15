#include "cvk/sync/fence.h"
#include "cvk/initialize/sync_initialize.h"



namespace cvk
{

Fence::Fence(VkDevice device, bool signaled) :
    _device(device),
    _signaled(signaled)
{
}

Fence::~Fence()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

VkResult Fence::create()
{
    return __cvk::create_fence(_device, object(), _signaled);
}

void Fence::set_create_signal(bool signaled)
{
    _signaled = signaled;
}

VkResult Fence::wait(uint32_t timeout)
{
    CVK_ASSERT(_device != VK_NULL_HANDLE);
    return __cvk::wait_fences(_device, { object() }, timeout);
}

VkResult Fence::reset()
{
    return __cvk::reset_fences(_device, { object() });
}

Fence::operator VkFence CONST_REFERENCE () const
{
    return object();
}

void Fence::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::destroy_fence(_device, object());
    }
}

};