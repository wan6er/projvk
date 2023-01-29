#include "cvk/command/base_command_buffer.h"
#include "cvk/command/base_command.h"
#include "base_command_buffer.inl"

namespace cvk
{

template class CVK_API BaseCommandBuffer<BaseCommandPrimary>;
template class CVK_API BaseCommandBuffer<BaseCommandSecondary>;

template class CVK_API BaseCommandBufferPrimary<BaseCommandPrimary>;
template class CVK_API BaseCommandBufferSecondary<BaseCommandSecondary>;


} // namespace cvk
