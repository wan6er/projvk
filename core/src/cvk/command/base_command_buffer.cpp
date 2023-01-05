#include "cvk/command/base_command_buffer.h"
#include "cvk/command/base_command.h"
#include "base_command_buffer.inl"

namespace cvk
{

template class CVK_API BaseCommandBuffer<CommandPrimary>;
template class CVK_API BaseCommandBuffer<CommandSecondary>;

template<>
CVK_API BaseCommandBuffer<CommandPrimary>::BaseCommandBuffer() :
    _cmd(object())
{
    __cvk::get_default_command_buffers_allocate_info(VK_NULL_HANDLE, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1, _allocate_info);
}

template<>
CVK_API BaseCommandBuffer<CommandSecondary>::BaseCommandBuffer() :
    _cmd(object())
{
    __cvk::get_default_command_buffers_allocate_info(VK_NULL_HANDLE, VK_COMMAND_BUFFER_LEVEL_SECONDARY, 1, _allocate_info);
}

} // namespace cvk
