#include "cvk/initialize/device_initialize.h"
#include "utils/vector_util.h"
#include "clog.h"



namespace __cvk
{

void get_device_queue_create_infos(const QueueIndexType& indices, uint32_t queue_flag, std::vector<VkDeviceQueueCreateInfo> &infos);

CVK_API void get_device_create_info(std::vector<VkDeviceQueueCreateInfo> CONST_REFERENCE queues, std::vector<char CONST_PTR> CONST_REFERENCE layers, std::vector<char CONST_PTR> CONST_REFERENCE exts, VkDeviceCreateInfo& create_info)
{
	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	utils::vector_fill_info(queues, create_info.queueCreateInfoCount, create_info.pQueueCreateInfos);
	utils::vector_fill_info(layers, create_info.enabledLayerCount, create_info.ppEnabledLayerNames);
	utils::vector_fill_info(exts, create_info.enabledExtensionCount, create_info.ppEnabledExtensionNames);
}

VkResult create_device(VkPhysicalDevice physical_device, VkDeviceCreateInfo CONST_REFERENCE create_info, VkDevice &device)
{
    // CVK_ASSERT(physical_device != VK_NULL_HANDLE);
	// std::vector<VkDeviceQueueCreateInfo> queue_infos;
	// get_device_queue_create_infos(indices, queue_flag, queue_infos);

	// std::vector<std::string> all_supported_extensions;
	// std::vector<std::string> supported_extensions;
	// std::vector<const char*> supported_extensions_pchar;
	// get_all_device_extensions(physical_device, all_supported_extensions);
    // utils::vector_filter<std::string>(all_supported_extensions, extensions_name, supported_extensions);
    // utils::string_to_charptr(supported_extensions, supported_extensions_pchar);

	// VkDeviceCreateInfo device_create_info = {};
	// device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	// device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_infos.size());
	// device_create_info.pQueueCreateInfos = queue_infos.size() == 0 ? nullptr : queue_infos.data();
	// device_create_info.enabledExtensionCount = static_cast<uint32_t>(supported_extensions_pchar.size());
	// device_create_info.ppEnabledExtensionNames = supported_extensions_pchar.size() == 0 ? nullptr : supported_extensions_pchar.data();
	// device_create_info.pEnabledFeatures = &features;

	return vkCreateDevice(physical_device, &create_info, nullptr, &device);
}

void destroy_device(VkDevice device)
{
	vkDestroyDevice(device, nullptr);
}

VkResult wait_device_idle(VkDevice device)
{
    return vkDeviceWaitIdle(device);
}

// void get_all_device_queue_families(VkPhysicalDevice device, std::vector<VkQueueFamilyProperties> &queue_families)
// {
// 	uint32_t count;
// 	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
// 	queue_families.resize(count);
// 	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queue_families.data());
// }

void get_all_device_extensions(VkPhysicalDevice device, std::vector<std::string> &extensions)
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

void get_device_queue(VkDevice device, uint32_t index, VkQueue& queue)
{
	vkGetDeviceQueue(device, index, 0, &queue);
}

void get_phydev_memory_properties(VkPhysicalDevice device, VkPhysicalDeviceMemoryProperties& properties)
{
	vkGetPhysicalDeviceMemoryProperties(device, &properties);
}

CVK_API void get_phydev_properties2(VkPhysicalDevice device, VkPhysicalDeviceProperties2& properties)
{
	properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    vkGetPhysicalDeviceProperties2(device, &properties);
}

// void get_all_device_queue_family_indices(VkPhysicalDevice device, QueueIndexType& queue_index)
// {
// 	std::vector<VkQueueFlagBits> flags = {
// 		VK_QUEUE_GRAPHICS_BIT,
// 		VK_QUEUE_COMPUTE_BIT,
// 		VK_QUEUE_TRANSFER_BIT,
// 		VK_QUEUE_SPARSE_BINDING_BIT,
// 	};
	
// 	for (auto flag : flags) {
// 		uint32_t index;
// 		get_device_queue_family(device, flag, index);
// 		queue_index.emplace(flag, index);
// 	}
// }

void get_device_queue_create_info(uint32_t index, VkDeviceQueueCreateInfo& info)
{
	const static float default_priority = 1.0f;
	info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	info.queueFamilyIndex = index;
	info.queueCount = 1;
	info.pQueuePriorities = &default_priority;
}

void get_device_queue_create_infos(const QueueIndexType& indices, uint32_t queue_flag, std::vector<VkDeviceQueueCreateInfo>& infos)
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

// void get_device_queue_family(VkPhysicalDevice device, VkQueueFlagBits queue_flag, uint32_t &index)
// {
// 	std::vector<VkQueueFamilyProperties> properties;
// 	get_all_device_queue_families(device, properties);

// 	// Dedicated queue for compute
// 	// Try to find a queue family index that supports compute but not graphics
// 	if (queue_flag & VK_QUEUE_COMPUTE_BIT)
// 	{
// 		for (uint32_t i = 0; i < static_cast<uint32_t>(properties.size()); i++)
// 		{
// 			if ((properties[i].queueFlags & queue_flag) && ((properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
// 			{
// 				index = i;
// 				return;
// 			}
// 		}
// 	}

// 	// Dedicated queue for transfer
// 	// Try to find a queue family index that supports transfer but not graphics and compute
// 	if (queue_flag & VK_QUEUE_TRANSFER_BIT)
// 	{
// 		for (uint32_t i = 0; i < static_cast<uint32_t>(properties.size()); i++)
// 		{
// 			if ((properties[i].queueFlags & queue_flag) && ((properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
// 			{
// 				index = i;
// 				return;
// 			}
// 		}
// 	}

// 	// For other queue types or if no separate compute queue is present, return the first one to support the requested flags
// 	for (uint32_t i = 0; i < static_cast<uint32_t>(properties.size()); i++)
// 	{
// 		if (properties[i].queueFlags & queue_flag)
// 		{
// 			index = i;
// 			return;
// 		}
// 	}
// }

// queue family
VkResult get_queue_family_properties(VkPhysicalDevice physical_device, std::vector<VkQueueFamilyProperties>& properties) 
{
    return query_properties(vkGetPhysicalDeviceQueueFamilyProperties, properties, physical_device);
}

VkResult get_queue_family_index_present_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface, uint32_t index, VkBool32 &support)
{
    return vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, index, surface, &support);
}

void get_queue_family_index(VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkQueueFamilyProperties> CONST_REFERENCE properties, VkQueueFlagBits queue_flag, uint32_t& index, uint32_t& present_index) 
{
    index = UINT32_MAX;
    present_index = UINT32_MAX;
    
    // std::vector<VkBool32> supports(properties.size());
    for (uint32_t i = 0; i < properties.size(); ++i) {
        if (properties[i].queueFlags & queue_flag) {
            index = i;
            VkBool32 both_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &both_support);
            if (both_support == VK_TRUE) {
                present_index = i;
            }
            break;
        }
    }
    if (present_index == UINT32_MAX) {
        for (uint32_t i = 0; i < properties.size(); ++i) {
            VkBool32 present_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &present_support);
            if (present_support == VK_TRUE) {
                present_index = i;
            }
        }

    }
}

void get_queue_family_index(std::vector<VkQueueFamilyProperties> CONST_REFERENCE properties, VkQueueFlagBits queue_flag, uint32_t &index, uint32_t offset)
{    
    index = UINT32_MAX;
    
    for (uint32_t i = offset; i < properties.size(); ++i) {
        if (properties[i].queueFlags & queue_flag) {
            index = i;
            break;
        }
    }
}

CVK_API void get_queue_family_index(VkPhysicalDevice physical_device, VkQueueFlagBits queue_flag, uint32_t& index, uint32_t offset)
{
    std::vector<VkQueueFamilyProperties> properties;
    get_queue_family_properties(physical_device, properties);
    return get_queue_family_index(properties, queue_flag, index, offset);
}

CVK_API void get_queue_family_index_present_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkQueueFlagBits queue_flag, uint32_t& index, uint32_t offset)
{
    std::vector<VkQueueFamilyProperties> properties;
    get_queue_family_properties(physical_device, properties);
    for (uint32_t i = offset; i < properties.size(); ++i) {
        get_queue_family_index(properties, queue_flag, i, i);
        VkBool32 supported = VK_FALSE;
        get_queue_family_index_present_support(physical_device, surface, i, supported);
        if (supported == VK_TRUE) {
            index = i;
            break;
        }
    }
}


};