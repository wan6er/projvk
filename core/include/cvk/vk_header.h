#pragma once

#include <vulkan/vulkan.h>

#if defined(_WIN32)

#include <windows.h>
#include <vulkan/vulkan_win32.h>

#endif

constexpr static const char* EngineName = "cvk";
constexpr static uint32_t VulkanApiVersion = VK_API_VERSION_1_0;