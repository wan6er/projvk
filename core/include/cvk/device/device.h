#pragma once

#include "utils/base_obj.h"
#include "cvk/vk_header.h"
#include "cvk/device/physical_device.h"
#include "cvk/device/device_feature.h"

#include "cvk/initialize/device_initialize.h"

#include "utils/cvk_iterator.h"

#include <vector>
#include <map>
#include <string>

namespace cvk
{

    class CVK_API Device : protected utils::BaseObj<VkDevice>, public PhysicalDevice
    {
    public:
        Device(VkPhysicalDevice physical_device, void CONST_PTR next = nullptr);
        Device(const Device& device) = default;
        ~Device();

        operator VkDevice() const;

        inline Device& add_extension(char CONST_PTR name) { _exts.push_back(name); return *this; }
        inline Device& add_queue(VkDeviceQueueCreateInfo CONST_REFERENCE info) { _queues.push_back(info); return *this; }
        Device& add_queue(uint32_t index);
        Device& add_queue_flags(VkQueueFlags flag);

        template<class..._ExtNames>
        void add_extensions(_ExtNames&&...exts);

        template<class _FeatType>
        Device& add_device_feature(DeviceFeature<_FeatType> const& feature);
        template<class..._Feats>
        void add_device_features(_Feats&&...feats);

        VkResult create();
        VkResult create(VkQueueFlags flag);

    protected:
        void release();
        void clean();

    private:
        // typedef std::map<VkQueueFlagBits, uint32_t> DeviceQueueFamilyIndexType;
        // DeviceQueueFamilyIndexType _indices;
        std::vector<VkDeviceQueueCreateInfo> _queues; 
        std::vector<char CONST_PTR> _layers;
        std::vector<char CONST_PTR> _exts;

        std::vector<void CONST_PTR> _feats;

        VkDeviceCreateInfo _create_info {};
    };

};  


#include "device.inl"