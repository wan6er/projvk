#pragma once

#include <vulkan/vulkan.h>

#define CVK ::cvk::
#define CONST_REFERENCE const &
#define CONST_PTR const *

#if defined(WIN32)

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


    #if !defined(NDEBUG)
        #include <iostream>
        // #define CVK_ASSERT(expression) do { if (!(expression)) {} } while (false)
        extern "C" __declspec(dllimport) void DebugBreak();
        #define CVK_PRINT_FILE_LINE(msg, ...) printf("%s:%d error %s :" msg "\n", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
        #define CVK_ASSERT(expression) do { if (!(expression)) { CVK_PRINT_FILE_LINE("%s", #expression); DebugBreak(); } } while (false)
    #else
        #define CVK_ASSERT(expression) do { if (!(expression)) {} } while (false)
    #endif

#elif defined(linux)

    #include <xcb/xcb.h>
    #include <vulkan/vulkan_xcb.h>

    #if defined(BUILD_CVK)
        #define CVK_API 
    #else
        #define CVK_API 
    #endif

    #if defined(NDEBUG)
        #define CVK_ASSERT(expression) do { if (!(expression)) {} } while (false)
    #else
        #include <iostream>
        #define CVK_PRINT_FILE_LINE(msg, ...) printf("%s:%d error %s :" msg "\n", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
        #define CVK_ASSERT(expression) do { if (!(expression)) { CVK_PRINT_FILE_LINE("%s", #expression); __builtin_trap(); } } while (false)
    #endif

#endif

constexpr static const char *EngineName = "cvk";
constexpr static uint32_t VulkanApiVersion = VK_API_VERSION_1_3;

#include <vector>

template <class _Func, class _Properties, class... _Args>
VkResult query_properties(_Func func, std::vector<_Properties> &properties, _Args &&...args);

#include "vk_header.inl"