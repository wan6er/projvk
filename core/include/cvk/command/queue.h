#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"
#include "utils/base_multi_attach.h"

namespace cvk
{
    
    class CVK_API Queue :
        public utils::BaseObj<VkQueue>
    {
    public:
        Queue(VkDevice device, uint32_t queue_index);
        virtual ~Queue();

        Queue& set_wait(VkPipelineStageFlags wait_stage, std::vector<VkSemaphore> CONST_REFERENCE wait);
        Queue& set_signal(std::vector<VkSemaphore> CONST_REFERENCE signal);

        VkResult submit(std::vector<VkCommandBuffer> CONST_REFERENCE buffers, VkFence signal);

        operator VkQueue CONST_REFERENCE () const;
    
    protected:
        Queue& clear_all();

    private:
        VkPipelineStageFlags _wait_stage;
        std::vector<VkSemaphore> _wait;
        std::vector<VkSemaphore> _signal;
    };

} // namespace cvk
