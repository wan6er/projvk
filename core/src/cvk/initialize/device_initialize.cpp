#include "cvk/initialize/device_initialize.h"

void __cvk::get_all_device(VkInstance instance, std::vector<VkPhysicalDevice>& devices)
{
    uint32_t gpu_count = 0;
	vkEnumeratePhysicalDevices(instance, &gpu_count, nullptr);
    devices.resize(gpu_count);
	if (gpu_count == 0) {
        return;
	}
	vkEnumeratePhysicalDevices(instance, &gpu_count, devices.data());
}