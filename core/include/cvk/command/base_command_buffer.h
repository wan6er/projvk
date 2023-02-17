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
        BaseCommandBuffer(VkCommandBuffer CONST_REFERENCE buffer);
        
        BaseCommandBuffer(BaseCommandBuffer CONST_REFERENCE copy) :
            utils::BaseObj<VkCommandBuffer>(copy),
            _cmd(copy)
        {
            std::cout << "secondary copy\n";
        }
        virtual ~BaseCommandBuffer();

        VkResult create();
        void end() const;
        auto cmd() const -> _CmdType CONST_REFERENCE;

        operator VkCommandBuffer CONST_REFERENCE () const;

    protected:
        void release();
        VkCommandBufferAllocateInfo _allocate_info = {};
        VkDevice _device;
        VkCommandPool _pool;
        _CmdType _cmd;

    };

    template<class _CmdType>
    class BaseCommandBufferPrimary : public BaseCommandBuffer<_CmdType>
    {
    public:
        BaseCommandBufferPrimary(VkDevice device, VkCommandPool pool);
        BaseCommandBufferPrimary(VkCommandBuffer CONST_REFERENCE buffer);
        virtual ~BaseCommandBufferPrimary() = default;
        
        VkResult begin(VkCommandBufferUsageFlags usage) const;

    };

    template<class _CmdType>
    class BaseCommandBufferSecondary : public BaseCommandBuffer<_CmdType>
    {
    public:
        BaseCommandBufferSecondary(VkDevice device, VkCommandPool pool);
        BaseCommandBufferSecondary(VkCommandBuffer CONST_REFERENCE buffer);
        virtual ~BaseCommandBufferSecondary() = default;
        
        VkResult begin(VkRenderPass renderpass, VkFramebuffer framebuffer, uint32_t subpass = 0, VkCommandBufferUsageFlags usage = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) const;

    };

} // namespace cvk
