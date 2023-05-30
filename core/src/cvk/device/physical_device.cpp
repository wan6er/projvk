#include "cvk/device/physical_device.h"
#include "cvk/initialize/device_initialize.h"

namespace cvk
{

PhysicalDevice::PhysicalDevice(VkPhysicalDevice physical_device) :
    _physical_device(physical_device)
{
}

auto PhysicalDevice::get_memory_properties() const -> VkPhysicalDeviceMemoryProperties
{
    VkPhysicalDeviceMemoryProperties memory_properties {};
    __cvk::get_phydev_memory_properties(_physical_device, memory_properties);
    return memory_properties;
}

auto PhysicalDevice::get_properties2() const -> VkPhysicalDeviceProperties2
{
    VkPhysicalDeviceProperties2 properties {};
    __cvk::get_phydev_properties2(_physical_device, properties);
    return properties;
}


auto PhysicalDevice::get_physical_device() const -> VkPhysicalDevice
{
    return _physical_device;
}

};

namespace cvk
{
    
auto PhysicalDevice::get_raytracing_properties() const -> VkPhysicalDeviceRayTracingPipelinePropertiesKHR
{
    VkPhysicalDeviceRayTracingPipelinePropertiesKHR rt_properties {};
    rt_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;
    VkPhysicalDeviceProperties2 properties {};
    properties.pNext = &rt_properties;
    __cvk::get_phydev_properties2(_physical_device, properties);
    return rt_properties;
}

} // namespace cvk
