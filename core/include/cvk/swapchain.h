#pragma once

#include "utils/base_obj.h"
#include "utils/base_multi_attach.h"
#include "cvk/vk_header.h"

namespace cvk
{
    class CVK_API Swapchain : 
        protected utils::BaseObj<VkSwapchainKHR>,
        protected utils::BaseMultipleAttaches<VkImage>
    {
    public:
        Swapchain(VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR> CONST_REFERENCE present_modes, VkSurfaceFormatKHR surface_format);
        Swapchain(VkDevice device, VkSwapchainCreateInfoKHR CONST_REFERENCE create_info);
        virtual ~Swapchain();

        inline void set_image_usage(VkImageUsageFlags usage) noexcept { _create_info.imageUsage |= usage; }

        VkResult create();
        VkResult recreate();
        VkResult recreate(VkSwapchainCreateInfoKHR CONST_REFERENCE create_info);

        VkSwapchainCreateInfoKHR& info();
        VkImageViewCreateInfo get_image_view_info() const;
        VkFormat get_format() const;
        VkExtent2D get_extent() const noexcept;
        auto get_images() const -> std::vector<VkImage> CONST_REFERENCE;

        // Preferred API: caller handles VK_SUBOPTIMAL_KHR / VK_ERROR_OUT_OF_DATE_KHR.
        VkResult acquire(uint32_t& image_index, VkSemaphore signal_semaphore, VkFence signal_fence = VK_NULL_HANDLE);
        // Legacy API kept for compatibility.
        uint32_t acquire(VkSemaphore signal_semaphore, VkFence signal_fence = VK_NULL_HANDLE);
        VkResult present(VkQueue queue, std::vector<VkSemaphore> CONST_REFERENCE wait);

        operator ObjType CONST_REFERENCE () const;

    protected:
        void init_info(VkPhysicalDevice physical_device, VkSurfaceKHR surface, const std::vector<VkPresentModeKHR>& present_modes, VkSurfaceFormatKHR surface_format);
        VkResult refresh_info();

        void release();
    private:
        VkSwapchainCreateInfoKHR _create_info;
        VkDevice _device = VK_NULL_HANDLE;
        VkPhysicalDevice _physical_device = VK_NULL_HANDLE;
        VkSurfaceKHR _surface = VK_NULL_HANDLE;
        std::vector<VkPresentModeKHR> _present_modes;
        VkSurfaceFormatKHR _surface_format {};
        uint32_t _cur_present_index = UINT32_MAX;
    };
};