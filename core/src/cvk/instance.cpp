#include "cvk/instance.h"

#include "cvk/initialize/instance_initialize.h"

cvk::Instance::Instance(const std::vector<std::string> &extensions_name, const std::vector<std::string> &layers_name)
{
    __cvk::create_instance(extensions_name, layers_name, _instance);
}

cvk::Instance::~Instance()
{
    __cvk::destroy_instance(_instance);
}

cvk::Instance::operator VkInstance() const
{
    return _instance;
}

auto cvk::Instance::get_all_physical_device() const -> std::vector<VkPhysicalDevice>
{
    std::vector<VkPhysicalDevice> physical_devices;
    __cvk::get_all_devices(_instance, physical_devices);
    return physical_devices;
}