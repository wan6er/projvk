
namespace cvk
{

    // template<class _Feature>
    // VkBaseOutStructure CONST_PTR DeviceFeature<_Feature>::get_next() const
    // {
    //     return reinterpret_cast<VkBaseOutStructure>(_feat.pNext);
    // }

    // template<class _Feature>
    // void DeviceFeature<_Feature>::set_next(void* next)
    // {
    //     _feat.pNext = next;
    // }

    template<class _Feature>
    _Feature CONST_REFERENCE DeviceFeature<_Feature>::get_feature() const
    {
        return _feat;
    }

    template<class _Feature>
    _Feature& DeviceFeature<_Feature>::get_feature()
    {
        return _feat;
    }
    
} // namespace cvk
