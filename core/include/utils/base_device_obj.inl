
namespace utils
{

    template<class _Ty>
    PFN_vkVoidFunction BaseDeviceObj<_Ty>::get_proc(DeviceStr name) const
    {
        auto iter = _funcs.find(name);
        if (iter == _funcs.end()) {
            return nullptr;
        }
        return iter->second;
    }

    template<class _Ty>
    void BaseDeviceObj<_Ty>::import_function(DeviceFuncList CONST_REFERENCE funcs)
    {
        for (auto& name : funcs) {
            _funcs.insert(DeviceFuncPair(name, cvk_get_addr(_device, name)));
        }
    }
    
} // namespace utils
