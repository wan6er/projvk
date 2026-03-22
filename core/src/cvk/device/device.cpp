#include "cvk/device/device.h"
#include "cvk/initialize/device_initialize.h"

namespace cvk
{


Device::Device(VkPhysicalDevice physical_device, void CONST_PTR next) :
    utils::BaseObj<VkDevice>(),
    PhysicalDevice(physical_device)
{
    _create_info.pNext = next;
}        

Device::~Device()
{
    if (isolated()) {
        release();
    }
}

void Device::release()
{
    __cvk::destroy_device(object());
    clean();
}

void Device::clean()
{
    for (auto feat : _feats) {
        if (feat.deleter && feat.data) {
            feat.deleter(feat.data);
        }
    }
    _feats.clear();
}

Device& Device::add_queue(uint32_t index)
{
    VkDeviceQueueCreateInfo info {};
    __cvk::get_device_queue_create_info(index, info);
    _queues.push_back(info);
    return *this;
}

Device& Device::add_queue_flags(VkQueueFlags flag)
{
    for (uint32_t i = 0; i < 4; ++i) {
        uint32_t bit = flag & (1 << i);
        if (bit) {
            uint32_t index = 0;
            __cvk::get_queue_family_index(get_physical_device(), static_cast<VkQueueFlagBits>(bit), index);
            add_queue(index);
        }
    }
    return *this;
}

VkResult Device::create()
{
    __cvk::get_device_create_info(_queues, _layers, _exts, _create_info);
    return __cvk::create_device(get_physical_device(), _create_info, object());
}

VkResult Device::create(VkQueueFlags flag)
{
    Device::add_queue_flags(flag);
    return Device::create();
}

VkResult Device::wait() const
{
    return __cvk::wait_device_idle(object());
}

Device::operator VkDevice() const
{
    return object();
}

// auto Device::get_queue_family_index(VkQueueFlagBits flag) const -> uint32_t
// {
//     auto iter = _indices.find(flag);
//     if (iter == _indices.end()) {
//         return static_cast<uint32_t>(-1);
//     }
//     return iter->second;
// }

// auto Device::get_queue(VkQueueFlagBits flag) const -> VkQueue
// {
//     auto index = get_queue_family_index(flag);
//     if (index == static_cast<uint32_t>(-1)) {
//         return nullptr;
//     }
//     VkQueue queue;
//     __cvk::get_device_queue(object(), index, queue);
//     return queue;
// }

};