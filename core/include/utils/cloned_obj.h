#pragma once

#include "cvk/vk_header.h"

namespace utils
{
    class CVK_API ClonedObj
    {
    public:
        ClonedObj() = default;
        constexpr explicit ClonedObj(bool is_cloned) : _is_cloned(is_cloned) {}
        constexpr explicit ClonedObj(const ClonedObj& obj) : _is_cloned(obj._is_cloned) {}

        virtual void clone() final;
        virtual bool is_cloned() final;
    
    private:
        bool _is_cloned = false;
    };
};

// #include "cloned_obj.inl"