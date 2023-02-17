#include "cvk/device.h"
#include "cvk/initialize/device_initialize.h"

namespace cvk
{


cvk::Device::Device(VkPhysicalDevice physical_device, const std::vector<std::string> &extensions_name, const VkPhysicalDeviceFeatures &features, uint32_t queue_flag) :
    utils::BaseObj<VkDevice>(),
    PhysicalDevice(physical_device)
{
    __cvk::get_all_device_queue_family_indices(physical_device, _indices);
    __cvk::create_device(physical_device, _indices, extensions_name, features, queue_flag, object());
}        

// cvk::Device::Device(const Device& device) :
//     utils::BaseObj<ObjType>(device),
//     PhysicalDevice(device)
// {
// }

cvk::Device::~Device()
{
    if (isolated()) {
        release();
    }
}

void cvk::Device::release()
{
    __cvk::destroy_device(object());
}

cvk::Device::operator VkDevice() const
{
    return object();
}

auto cvk::Device::get_queue_family_index(VkQueueFlagBits flag) const -> uint32_t
{
    auto iter = _indices.find(flag);
    if (iter == _indices.end()) {
        return static_cast<uint32_t>(-1);
    }
    return iter->second;
}

auto cvk::Device::get_queue(VkQueueFlagBits flag) const -> VkQueue
{
    auto index = get_queue_family_index(flag);
    if (index == static_cast<uint32_t>(-1)) {
        return nullptr;
    }
    VkQueue queue;
    __cvk::get_device_queue(object(), index, queue);
    return queue;
}

};