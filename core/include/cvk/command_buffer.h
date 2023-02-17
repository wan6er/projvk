#pragma once

#include "cvk/vk_header.h"
#include "cvk/command/base_command.h"
#include "cvk/command/base_command_buffer.h"
#include "cvk/command/base_command_buffer_set.h"
namespace cvk
{
    
    typedef BaseCommandBufferPrimary<BaseCommandPrimary> CommandBuffer;
    typedef BaseCommandBufferSecondary<BaseCommandSecondary> CommandBufferSecondary;

    typedef BaseCommandBufferSet<CommandBuffer, VK_COMMAND_BUFFER_LEVEL_PRIMARY> CommandBufferSet;
    typedef BaseCommandBufferSet<CommandBufferSecondary, VK_COMMAND_BUFFER_LEVEL_SECONDARY> CommandBufferSecondarySet;

} // namespace cvk
