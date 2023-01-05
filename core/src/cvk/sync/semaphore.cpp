#include "cvk/sync/semaphore.h"
#include "cvk/initialize/sync_initialize.h"

namespace cvk
{

Semaphore::Semaphore(bool signaled) :
    _signaled(signaled)
{
}

Semaphore::~Semaphore()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

VkResult Semaphore::create(VkDevice device)
{
    _device = device;
    return __cvk::create_semaphore(_device, object(), _signaled);
}

Semaphore::operator VkSemaphore CONST_REFERENCE () const
{
    return object();
}

void Semaphore::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::destroy_semaphore(_device, object());
    }
}

};