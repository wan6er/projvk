#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"

#include <unordered_map>
#include <string_view>

namespace utils
{

    using DeviceStr = std::string_view;
    using DeviceFuncList = std::vector<DeviceStr>;
    using DeviceFuncMap = std::unordered_map<DeviceStr, PFN_vkVoidFunction>;
    using DeviceFuncPair = std::pair<DeviceStr, PFN_vkVoidFunction>;

    template<class _Ty>
    class BaseDeviceObj : public utils::BaseObj<_Ty>
    {
        using _BaseObj = utils::BaseObj<_Ty>;
    public:
        explicit BaseDeviceObj(VkDevice device) : 
            _BaseObj(), _device(device) 
        { 
        }

        template<class..._Args>
        explicit BaseDeviceObj(VkDevice device, _Args&&...args) : 
            _BaseObj(std::forward<_Args>(args)...), _device(device)
        {
        }

        BaseDeviceObj(BaseDeviceObj CONST_REFERENCE) = default;

    protected:
        void import_function(DeviceFuncList CONST_REFERENCE funcs);
        PFN_vkVoidFunction get_proc(DeviceStr name) const;
        inline DeviceFuncMap get_procs() const noexcept { return _funcs; }
        inline VkDevice device() const noexcept { return _device; }

    private:
        VkDevice _device {};
        DeviceFuncMap _funcs {};

    };
    
} // namespace utils

#include "base_device_obj.inl"