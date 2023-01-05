#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"

namespace cvk
{
    
    class CVK_API CommandPool :
        public utils::BaseObj<VkCommandPool>
    {
    public:
        CommandPool(uint32_t queue_family);
        virtual ~CommandPool();

        CommandPool& transient();

        VkResult create(VkDevice device);

        operator VkCommandPool CONST_REFERENCE () const;

    protected:
        void release();

    private:
        VkCommandPoolCreateInfo _create_info;
        VkDevice _device = VK_NULL_HANDLE;

    };

} // namespace cvk
