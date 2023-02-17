#include "cvk/fence.h"
#include "cvk/initialize/sync_initialize.h"
#include "utils/vector_util.h"

namespace cvk
{
    
template<>
VkResult FenceSet::wait(uint32_t timeout)
{
    std::vector<Fence> CONST_REFERENCE fences = *this;
    std::vector<VkFence> _temp;
    std::function transitional = [] (Fence CONST_REFERENCE src, VkFence& dst) -> void {
        dst = src;
    };
    utils::vector_transition(fences, _temp, transitional);
    return __cvk::wait_fences(_device, _temp, timeout);
}

} // namespace cvk
