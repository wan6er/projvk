#include "utils/cvk_iterator.h"

namespace cvk
{

    static inline void without_parameter(...) {}

    template<class..._ExtNames>
    void Device::add_extensions(_ExtNames&&...exts)
    {
        without_parameter(add_extension(std::forward<_ExtNames>(exts))...);
    }

    // template<class..._Queues>
    // void Device::add_queues(_Queues&&...queues)
    // {
    //     without_parameter(add_queue(std::forward<_Queues>(queues))...);
    // }
    
    template<class _FeatType>
    Device& Device::add_device_feature(DeviceFeature<_FeatType> const& feature)
    {
        _feats.emplace_back(new DeviceFeature<_FeatType>(feature));

        utils::CvkIterator iter(&_create_info);
        while (iter.has_next()) ++iter;

        iter->pNext = (utils::CvkIterator::CvkIteratorPtr) _feats.back();
        return *this;
    }

    template<class..._Feats>
    void Device::add_device_features(_Feats&&...feats)
    {
        without_parameter(add_device_feature(feats)...);
    }
    
} // namespace cvk
