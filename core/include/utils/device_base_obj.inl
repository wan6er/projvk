#pragma once

namespace utils
{


template<class _ObjType>
DeviceBaseObj<_ObjType>::DeviceBaseObj(std::function<void ()> release_callback) :
    release_callback(release_callback)
{
}

template<class _ObjType>
DeviceBaseObj<_ObjType>::~DeviceBaseObj()
{
    if (this->isolated() && !this->is_cloned()) {
        release_callback();
    }
}

template<class _ObjType>
DeviceBaseObj<_ObjType>::operator _ObjType CONST_REFERENCE () const
{
    return this->object();
}

template<class _ObjType>
DeviceBaseObj<_ObjType>::operator _ObjType& ()
{
    return this->object();
}

template<class _ObjType>
VkDevice& DeviceBaseObj<_ObjType>::device()
{
    return _device;
}


};