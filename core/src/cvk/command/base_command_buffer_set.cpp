#include "cvk/command_buffer.h"
#include "cvk/command/base_command_buffer_set.h"
#include "cvk/initialize/command_initialize.h"
#include "base_command_buffer_set.inl"

namespace cvk
{

template class CVK_API BaseCommandBufferSet<CommandBufferSecondary, VK_COMMAND_BUFFER_LEVEL_SECONDARY>;
template class CVK_API BaseCommandBufferSet<CommandBuffer, VK_COMMAND_BUFFER_LEVEL_PRIMARY>;

}