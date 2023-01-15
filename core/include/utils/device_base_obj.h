#pragma once

#include "cvk/vk_header.h"
#include "base_obj.h"

#include <functional>

namespace utils
{

    template<class _ObjType>
    class DeviceBaseObj : public BaseObj<_ObjType>
    {
    public:
        DeviceBaseObj(std::function<void()> release_callback);
        virtual ~DeviceBaseObj();

        operator _ObjType CONST_REFERENCE () const;
        operator _ObjType& ();
    
    protected:
        VkDevice& device();
        std::function<void ()> release_callback;

    private:
        VkDevice _device;

    };
    

};

#include "device_base_obj.inl"