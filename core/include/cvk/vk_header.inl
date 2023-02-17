#pragma once

#include <iostream>
#include <type_traits>

template<class _Func, class _Properties, class..._Args>
VkResult query_properties(_Func func, std::vector<_Properties>& properties, _Args&&... args)
{
    uint32_t count = 0;
    
    constexpr bool no_ret = std::is_same<decltype(func(std::forward<_Args>(args)..., &count, nullptr)), void>::value;
    VkResult result = VK_SUCCESS;

    if constexpr (no_ret) {
        func(std::forward<_Args>(args)..., &count, nullptr);
    } else {
        result = func(std::forward<_Args>(args)..., &count, nullptr);
    }
    
    if (result == VK_SUCCESS && count > 0) {
        properties.resize(count);
        if constexpr (no_ret) {
            func(std::forward<_Args>(args)..., &count, properties.data());
        } else {
            result = func(std::forward<_Args>(args)..., &count, properties.data());
        }
    }
    return result;
}