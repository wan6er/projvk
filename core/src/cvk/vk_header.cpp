#include "cvk/vk_header.h"

template<>
PFN_vkVoidFunction cvk_get_addr<char CONST_PTR>(VkDevice device, char CONST_PTR str)
{
    return vkGetDeviceProcAddr(device, str);
}

template<>
PFN_vkVoidFunction cvk_get_addr<std::string_view>(VkDevice device, std::string_view str)
{
    return vkGetDeviceProcAddr(device, str.data());
}

CVK_API size_t cvk_aligned_size(size_t size, size_t align)
{
    return (size + align - 1) & ~(align - 1);
}