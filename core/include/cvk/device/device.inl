#include "utils/cvk_iterator.h"

namespace cvk
{
    template<class..._ExtNames>
    void Device::add_extensions(_ExtNames&&...exts)
    {
        ((void)add_extension(std::forward<_ExtNames>(exts)), ...);
    }
    
    template<class _FeatType>
    Device& Device::add_device_feature(DeviceFeature<_FeatType> const& feature)
    {
        auto* feat = new _FeatType(feature.get_feature());
        _feats.emplace_back(FeatureStorage{
            feat,
            [](void* ptr) { delete static_cast<_FeatType*>(ptr); },
            reinterpret_cast<VkBaseOutStructure*>(feat)
        });

        utils::CvkIterator iter(&_create_info);
        while (iter.has_next()) ++iter;

        iter->pNext = _feats.back().base;
        return *this;
    }

    template<class..._Feats>
    void Device::add_device_features(_Feats&&...feats)
    {
        ((void)add_device_feature(std::forward<_Feats>(feats)), ...);
    }
    
} // namespace cvk
