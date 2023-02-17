#include "cvk/initialize/instance_initialize.h"

#include "clog.h"
#include "utils/vector_util.h"

#include <algorithm>

void __cvk::get_all_devices(VkInstance instance, std::vector<VkPhysicalDevice> &devices)
{
	// uint32_t gpu_count = 0;
	// vkEnumeratePhysicalDevices(instance, &gpu_count, nullptr);
	// devices.resize(gpu_count);
	// if (gpu_count == 0)
	// {
	// 	return;
	// }
	// vkEnumeratePhysicalDevices(instance, &gpu_count, devices.data());
    query_properties(vkEnumeratePhysicalDevices, devices, instance);
}

void __cvk::get_all_instance_extensions(std::vector<std::string>& supported_extensions) 
{
    // uint32_t ext_count = 0;
    // vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, nullptr);
    // if (ext_count > 0)
    // {
    //     std::vector<VkExtensionProperties> extensions(ext_count);
    //     if (vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, &extensions.front()) == VK_SUCCESS)
    //     {
    //         for (auto& extension : extensions)
    //         {
    //             supported_extensions.push_back(extension.extensionName);
    //         }
    //     }
    // }
    std::vector<VkExtensionProperties> extensions;
    query_properties(vkEnumerateInstanceExtensionProperties, extensions, nullptr);
    for (auto& extension : extensions)
    {
        supported_extensions.push_back(extension.extensionName);
    }
}

void __cvk::get_all_instance_layers(std::vector<std::string>& supported_layers) 
{
    // uint32_t layer_count = 0;
    // vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    // if (layer_count > 0)
    // {
    //     std::vector<VkLayerProperties> layers(layer_count);
    //     if (vkEnumerateInstanceLayerProperties(&layer_count, &layers.front()) == VK_SUCCESS)
    //     {
    //         for (auto layer : layers)
    //         {
    //             supported_layers.push_back(layer.layerName);
    //         }
    //     }
    // }    
    std::vector<VkLayerProperties> layers;
    query_properties(vkEnumerateInstanceLayerProperties, layers);
    for (auto layer : layers)
    {
        supported_layers.push_back(layer.layerName);
    }
}

VkResult __cvk::create_instance(std::vector<std::string> CONST_REFERENCE extensions_name, std::vector<std::string> CONST_REFERENCE layers_name, VkInstance& instance)
{
	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = EngineName;
	app_info.pEngineName = EngineName;
	app_info.apiVersion = VulkanApiVersion;
    
	std::vector<std::string> all_supported_extensions;
	std::vector<std::string> supported_extensions;
	std::vector<const char*> supported_extensions_pchar;
    get_all_instance_extensions(all_supported_extensions);
    utils::vector_filter<std::string>(all_supported_extensions, extensions_name, supported_extensions);
    utils::string_to_charptr(supported_extensions, supported_extensions_pchar);

	std::vector<std::string> all_supported_layers;
	std::vector<std::string> supported_layers;
	std::vector<const char*> supported_layers_pchar;
    get_all_instance_layers(all_supported_layers);
    utils::vector_filter<std::string>(all_supported_layers, layers_name, supported_layers);
    utils::string_to_charptr(supported_layers, supported_layers_pchar);

    VkInstanceCreateInfo instance_create_info = {};
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pNext = NULL;
    instance_create_info.pApplicationInfo = &app_info;
    instance_create_info.enabledExtensionCount = (uint32_t)supported_extensions_pchar.size();
    instance_create_info.ppEnabledExtensionNames = supported_extensions_pchar.data();
    instance_create_info.enabledLayerCount = (uint32_t)supported_layers_pchar.size();
    instance_create_info.ppEnabledLayerNames = supported_layers_pchar.data();

	return vkCreateInstance(&instance_create_info, nullptr, &instance);
}

void __cvk::destroy_instance(VkInstance instance)
{
    vkDestroyInstance(instance, nullptr);
}