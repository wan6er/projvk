#include "cvk/command/queue.h"
#include "cvk/initialize/command_initialize.h"

namespace cvk
{

Queue::Queue(VkDevice device, uint32_t queue_index)
{
    __cvk::get_queue(device, queue_index, object());
}

Queue::~Queue()
{

}

Queue& Queue::set_wait(VkPipelineStageFlags wait_stage, std::vector<VkSemaphore> CONST_REFERENCE wait)
{
    CVK_ASSERT(wait.size() > 0);
    _wait_stage = wait_stage;
    _wait = wait;
    return *this;
}

Queue& Queue::set_signal(std::vector<VkSemaphore> CONST_REFERENCE signal)
{
    CVK_ASSERT(signal.size() > 0);
    _signal = signal;
    return *this;
}

Queue& Queue::clear_all()
{
    _wait.clear();
    _signal.clear();
    return *this;
}

VkResult Queue::submit(std::vector<VkCommandBuffer> CONST_REFERENCE buffers, VkFence signal)
{
    VkResult result = __cvk::queue_submit(_wait_stage, _wait, _signal, signal, buffers, object());
    clear_all();
    return result;
}

Queue::operator VkQueue CONST_REFERENCE () const
{
    return object();
}

} // namespace cvk
