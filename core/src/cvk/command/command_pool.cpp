#include "cvk/command/command_pool.h"
#include "cvk/initialize/command_initialize.h"



namespace cvk
{
    
CommandPool::CommandPool(VkDevice device, uint32_t queue_family) :
    _device(device)
{
    __cvk::get_default_command_pool_create_info(queue_family, _create_info);
}

CommandPool::~CommandPool()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

CommandPool& CommandPool::transient()
{
    CVK_ASSERT(object() == VK_NULL_HANDLE);
    _create_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    return *this;
}

VkResult CommandPool::create()
{
    return __cvk::create_command_pool(_device, _create_info, object());
}

CommandPool::operator VkCommandPool CONST_REFERENCE () const
{
    CVK_ASSERT(object() != VK_NULL_HANDLE);
    return object();
}

void CommandPool::release()
{
    if (_device != VK_NULL_HANDLE && object() != VK_NULL_HANDLE) {
        __cvk::destroy_command_pool(_device, object());
    }
}

} // namespace cvk
