#include "cvk/instance.h"

#include "cvk/initialize/instance_initialize.h"

cvk::Instance::Instance(const std::vector<std::string> &extensions_name, const std::vector<std::string> &layers_name)
{
    __cvk::create_instance(extensions_name, layers_name, object());
}

cvk::Instance::~Instance()
{
}

void cvk::Instance::release()
{
    __cvk::destroy_instance(object());
}

cvk::Instance::operator VkInstance() const
{
    return object();
}

auto cvk::Instance::get_all_physical_device() const -> std::vector<VkPhysicalDevice>
{
    std::vector<VkPhysicalDevice> physical_devices;
    __cvk::get_all_devices(object(), physical_devices);
    return physical_devices;
}