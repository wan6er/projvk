#pragma once

#include "cvk/vk_header.h"
#include "base_command.h"
#include "base_command_buffer.h"

namespace cvk
{
    
    typedef BaseCommandBuffer<CommandPrimary> CommandBuffer;
    typedef BaseCommandBuffer<CommandSecondary> CommandBufferSecondary;

} // namespace cvk
