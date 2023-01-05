#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"

namespace cvk
{

    template<class _CmdType>
    class BaseCommandBuffer : public utils::BaseObj<VkCommandBuffer>
    {
    public:
        BaseCommandBuffer();
        virtual ~BaseCommandBuffer();

        VkResult create(VkDevice device, VkCommandPool pool);

        VkResult begin(VkCommandBufferUsageFlags usage);
        void end();

        auto cmd() const -> _CmdType CONST_REFERENCE;

        operator VkCommandBuffer CONST_REFERENCE () const;

    protected:
        void release();
        VkCommandBufferAllocateInfo _allocate_info = {};
        VkCommandPool _pool = VK_NULL_HANDLE;
        VkDevice _device = VK_NULL_HANDLE;
        _CmdType _cmd;

    };

} // namespace cvk


// #include "base_command_buffer.inl"