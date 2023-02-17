#pragma once

#include <vulkan/vulkan.h>

#define CVK ::cvk::
#define CONST_REFERENCE const &
#define CONST_PTR const *

#if defined(_WIN32)

#include <windows.h>
#include <vulkan/vulkan_win32.h>

#if defined(BUILD_CVK)
#define CVK_API __declspec(dllexport)
// #define CVK_API [[gnu::dllexport]]
#else
#define CVK_API __declspec(dllimport)
// #define CVK_API [[gnu::dllimport]]
// #define CVK_API
#endif

#endif

#if defined(NDEBUG)

#define CVK_ASSERT(expression) ((void)0)

#elif !defined(NDEBUG) && defined(WIN32)

extern "C" __declspec(dllimport) void DebugBreak();

#define CVK_PRINT_FILE_LINE(msg, ...) printf("%s:%d error %s :" msg "\n", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define CVK_ASSERT(expression) do { if (!(expression)) { CVK_PRINT_FILE_LINE("%s", #expression); DebugBreak(); } } while (false)

#endif

constexpr static const char *EngineName = "cvk";
constexpr static uint32_t VulkanApiVersion = VK_API_VERSION_1_3;

#include <vector>

template <class _Func, class _Properties, class... _Args>
VkResult query_properties(_Func func, std::vector<_Properties> &properties, _Args &&...args);

#include "vk_header.inl"