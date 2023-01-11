#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"

namespace cvk
{

    template<class _CmdType>
    class BaseCommandBuffer : public utils::BaseObj<VkCommandBuffer>
    {
    public:
        BaseCommandBuffer(VkDevice device, VkCommandPool pool);
        virtual ~BaseCommandBuffer();

        VkResult create();

        VkResult begin(VkCommandBufferUsageFlags usage);
        void end();

        auto cmd() const -> _CmdType CONST_REFERENCE;

        operator VkCommandBuffer CONST_REFERENCE () const;

    protected:
        void release();
        VkCommandBufferAllocateInfo _allocate_info = {};
        VkDevice _device;
        VkCommandPool _pool;
        _CmdType _cmd;

    };

} // namespace cvk


// #include "base_command_buffer.inl"