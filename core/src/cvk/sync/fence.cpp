#include "cvk/sync/fence.h"
#include "cvk/initialize/sync_initialize.h"



namespace cvk
{

Fence::Fence(bool signaled) :
    _signaled(signaled)
{
}

Fence::~Fence()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

VkResult Fence::create(VkDevice device)
{
    _device = device;
    return __cvk::create_fence(_device, object(), _signaled);
}

VkResult Fence::wait(uint32_t timeout)
{
    CVK_ASSERT(_device != VK_NULL_HANDLE);
    return vkWaitForFences(_device, 1, &object(), VK_TRUE, timeout);
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