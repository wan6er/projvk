#include "cvk/initialize/device_initialize.h"

#include "utils/vector_util.h"
#include "clog.h"

void get_device_queue_create_info(uint32_t index, VkDeviceQueueCreateInfo& info);
void get_device_queue_create_infos(const __cvk::QueueIndexType& indices, uint32_t queue_flag, std::vector<VkDeviceQueueCreateInfo> &infos);
void get_device_queue_family(VkPhysicalDevice device, VkQueueFlagBits queue_flag, uint32_t &index);


VkResult __cvk::create_device(VkPhysicalDevice physical_device, const __cvk::QueueIndexType& indices, const std::vector<std::string> &extensions_name, const VkPhysicalDeviceFeatures& features, uint32_t queue_flag, VkDevice &device)
{
	std::vector<VkDeviceQueueCreateInfo> queue_infos;
	get_device_queue_create_infos(indices, queue_flag, queue_infos);

	std::vector<std::string> all_supported_extensions;
	std::vector<std::string> supported_extensions;
	std::vector<const char*> supported_extensions_pchar;
	__cvk::get_all_device_extensions(physical_device, all_supported_extensions);
    utils::filter_vector<std::string>(all_supported_extensions, extensions_name, supported_extensions);
    utils::string_to_charptr(supported_extensions, supported_extensions_pchar);

	VkDeviceCreateInfo device_create_info = {};
	device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_infos.size());
	device_create_info.pQueueCreateInfos = queue_infos.data();
	device_create_info.enabledExtensionCount = static_cast<uint32_t>(supported_extensions_pchar.size());
	device_create_info.ppEnabledExtensionNames = supported_extensions_pchar.data();
	device_create_info.pEnabledFeatures = &features;

	return vkCreateDevice(physical_device, &device_create_info, nullptr, &device);
}

void __cvk::destroy_device(VkDevice device)
{
	vkDestroyDevice(device, nullptr);
}

void __cvk::get_all_device_queue_families(VkPhysicalDevice device, std::vector<VkQueueFamilyProperties> &queue_families)
{
	uint32_t count;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
	queue_families.resize(count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queue_families.data());
}

void __cvk::get_all_device_extensions(VkPhysicalDevice device, std::vector<std::string> &extensions)
{
	uint32_t count = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);
	if (count > 0)
	{
		std::vector<VkExtensionProperties> extension_properties(count);
		if (vkEnumerateDeviceExtensionProperties(device, nullptr, &count, &extension_properties.front()) == VK_SUCCESS)
		{
			for (auto ext : extension_properties)
			{
				extensions.push_back(ext.extensionName);
			}
		}
	}
}

void __cvk::get_device_queue(VkDevice device, uint32_t index, VkQueue& queue)
{
	vkGetDeviceQueue(device, index, 0, &queue);
}

void __cvk::get_device_memory_properties(VkPhysicalDevice device, VkPhysicalDeviceMemoryProperties& properties)
{
	vkGetPhysicalDeviceMemoryProperties(device, &properties);
}

void __cvk::get_all_device_queue_family_indices(VkPhysicalDevice device, QueueIndexType& queue_index)
{
	std::vector<VkQueueFlagBits> flags = {
		VK_QUEUE_GRAPHICS_BIT,
		VK_QUEUE_COMPUTE_BIT,
		VK_QUEUE_TRANSFER_BIT,
		VK_QUEUE_SPARSE_BINDING_BIT,
	};
	
	for (auto flag : flags) {
		uint32_t index;
		get_device_queue_family(device, flag, index);
		queue_index.emplace(flag, index);
	}
}

void get_device_queue_create_info(uint32_t index, VkDeviceQueueCreateInfo& info)
{
	const static float default_priority = 1.0f;
	info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	info.queueFamilyIndex = index;
	info.queueCount = 1;
	info.pQueuePriorities = &default_priority;
}

void get_device_queue_create_infos(const __cvk::QueueIndexType& indices, uint32_t queue_flag, std::vector<VkDeviceQueueCreateInfo>& infos)
{
	for (auto iter = indices.begin(); iter != indices.end(); ++iter) 
	{
		if (queue_flag & iter->first) {
			VkDeviceQueueCreateInfo info = {};
			get_device_queue_create_info(iter->second, info);
			infos.push_back(info);
		}
	}
}

void get_device_queue_family(VkPhysicalDevice device, VkQueueFlagBits queue_flag, uint32_t &index)
{
	std::vector<VkQueueFamilyProperties> properties;
	__cvk::get_all_device_queue_families(device, properties);

	// Dedicated queue for compute
	// Try to find a queue family index that supports compute but not graphics
	if (queue_flag & VK_QUEUE_COMPUTE_BIT)
	{
		for (uint32_t i = 0; i < static_cast<uint32_t>(properties.size()); i++)
		{
			if ((properties[i].queueFlags & queue_flag) && ((properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
			{
				index = i;
				return;
			}
		}
	}

	// Dedicated queue for transfer
	// Try to find a queue family index that supports transfer but not graphics and compute
	if (queue_flag & VK_QUEUE_TRANSFER_BIT)
	{
		for (uint32_t i = 0; i < static_cast<uint32_t>(properties.size()); i++)
		{
			if ((properties[i].queueFlags & queue_flag) && ((properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
			{
				index = i;
				return;
			}
		}
	}

	// For other queue types or if no separate compute queue is present, return the first one to support the requested flags
	for (uint32_t i = 0; i < static_cast<uint32_t>(properties.size()); i++)
	{
		if (properties[i].queueFlags & queue_flag)
		{
			index = i;
			return;
		}
	}
}