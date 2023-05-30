#pragma once

#include "cvk/vk_header.h"
#include "cvk/command/base_command.h"
#include "utils/base_device_obj.h"

namespace cvk
{

    class CVK_API BaseCommandBuffer : public utils::BaseDeviceObj<VkCommandBuffer>
    {
        using _CmdBufBase = utils::BaseDeviceObj<VkCommandBuffer>;

    public:
        BaseCommandBuffer(VkDevice device, VkCommandPool pool);
        // BaseCommandBuffer(VkCommandBuffer CONST_REFERENCE buffer);
        
        BaseCommandBuffer(BaseCommandBuffer CONST_REFERENCE copy) = default;
        virtual ~BaseCommandBuffer();

        VkResult create();
        void end() const;
        // auto cmd() const -> _CmdType;

        operator VkCommandBuffer() const;

    protected:
        void release();

        VkCommandBufferAllocateInfo _allocate_info = {};
        // VkDevice _device;
        VkCommandPool _pool;

    };

    class CVK_API CommandBuffer : public BaseCommandBuffer
    {
    public:
        CommandBuffer(VkDevice device, VkCommandPool pool);
        // CommandBuffer(VkCommandBuffer CONST_REFERENCE buffer);
        virtual ~CommandBuffer() = default;

        VkResult begin(VkCommandBufferUsageFlags usage = 0) const;
        inline auto cmd() const { return CommandPrimary(device(), object()); }

    };

    class CVK_API CommandBufferSecondary : public BaseCommandBuffer
    {
    public:
        CommandBufferSecondary(VkDevice device, VkCommandPool pool);
        // CommandBufferSecondary(VkCommandBuffer CONST_REFERENCE buffer);
        virtual ~CommandBufferSecondary() = default;
        
        VkResult begin(VkRenderPass renderpass, VkFramebuffer framebuffer, uint32_t subpass = 0, VkCommandBufferUsageFlags usage = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) const;
        inline auto cmd() const { return CommandSecondary(device(), object()); }

    };

} // namespace cvk
