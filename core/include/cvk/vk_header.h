#pragma once

#include <vulkan/vulkan.h>

#define CONST_REFERENCE const &
// #define RIGHT_REFERENCE &&
#define CONST_PTR const *

#if defined(_WIN32)

#include <windows.h>
#include <vulkan/vulkan_win32.h>

#if defined(BUILD_CVK)
#define CVK_API __declspec(dllexport)
#else
#define CVK_API __declspec(dllimport)
// #define CVK_API
#endif

#endif

#if defined(NDEBUG)

#define CVK_ASSERT(expression) ((void)0)

#elif !defined(NDEBUG) && defined(WIN32)

extern "C" __declspec(dllimport) void DebugBreak();

#define CVK_ASSERT(expression) do { if (!((bool)(expression))) ::DebugBreak(); } while (false)

#endif

constexpr static const char *EngineName = "cvk";
constexpr static uint32_t VulkanApiVersion = VK_API_VERSION_1_3;

#include <vector>

template <class _Func, class _Properties, class... _Args>
VkResult query_properties(_Func func, std::vector<_Properties> &properties, _Args &&...args);

#include "vk_header.inl"