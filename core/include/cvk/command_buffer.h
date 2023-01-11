#pragma once

#include "cvk/vk_header.h"
#include "cvk/command/base_command.h"
#include "cvk/command/base_command_buffer.h"

namespace cvk
{
    
    typedef BaseCommandBuffer<CommandPrimary> CommandBuffer;
    typedef BaseCommandBuffer<CommandSecondary> CommandBufferSecondary;

} // namespace cvk
