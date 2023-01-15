#include "cvk/physical_device.h"
#include "cvk/initialize/device_initialize.h"

namespace cvk
{

cvk::PhysicalDevice::PhysicalDevice(VkPhysicalDevice physical_device) :
    _physical_device(physical_device)
{
    __cvk::get_device_memory_properties(_physical_device, _memory_properties);
}

cvk::PhysicalDevice::PhysicalDevice(const PhysicalDevice& physical_device)
{
    _physical_device = physical_device.get_physical_device();
    _memory_properties = physical_device.get_memory_properties();
}

auto cvk::PhysicalDevice::get_memory_properties() const -> const VkPhysicalDeviceMemoryProperties &
{
    return _memory_properties;
}

auto cvk::PhysicalDevice::get_physical_device() const -> VkPhysicalDevice
{
    return _physical_device;
}

};