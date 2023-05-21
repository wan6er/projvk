#pragma once

#include "cvk/vk_header.h"

namespace cvk
{

    template<class _Feature>
    class DeviceFeature
    {
        // static_assert(offsetof(_Feature, pNext) > 0);

    public:
        // VkBaseOutStructure CONST_PTR get_next() const;

    // protected:
        _Feature CONST_REFERENCE get_feature() const;
        _Feature& get_feature();

    private:
        _Feature _feat {};

    };
    
} // namespace cvk

#include "device_feature.inl"