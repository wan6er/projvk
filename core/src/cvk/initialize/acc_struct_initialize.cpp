#include "cvk/initialize/acc_struct_initialize.h"

#include "utils/vector_util.h"

namespace __cvk
{

CVK_API void get_default_acc_struct_build_geometry_info(VkAccelerationStructureTypeKHR type, VkFlags flags, std::vector<VkAccelerationStructureGeometryKHR> CONST_REFERENCE geometries, VkAccelerationStructureBuildGeometryInfoKHR& info)
{
    info = {};
    info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    info.type = type;
    info.flags = flags;
    utils::vector_fill_info(geometries, info.geometryCount, info.pGeometries);
}

CVK_API void get_acc_struct_geometries_size(VkDevice device, VkAccelerationStructureBuildTypeKHR type, VkAccelerationStructureBuildGeometryInfoKHR CONST_REFERENCE geometries, VkAccelerationStructureBuildSizesInfoKHR& size)
{
    size.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
    CVK_IMPORT_FUNCTION(device, vkGetAccelerationStructureBuildSizesKHR);
    vkGetAccelerationStructureBuildSizesKHR(device, type, &geometries, &geometries.geometryCount, &size);
}

CVK_API void get_default_acc_struct_create_info(VkAccelerationStructureTypeKHR type, VkBuffer buffer, uint64_t size, VkAccelerationStructureCreateInfoKHR& info)
{
    info = {};
    info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
    info.buffer = buffer;
    info.size = size;
    info.type = type;
}

CVK_API VkResult create_acceleration_structure(VkDevice device, VkAccelerationStructureCreateInfoKHR CONST_REFERENCE info, VkAccelerationStructureKHR& acc_struct)
{
    CVK_IMPORT_FUNCTION(device, vkCreateAccelerationStructureKHR);
    return vkCreateAccelerationStructureKHR(device, &info, nullptr, &acc_struct);
}

CVK_API void destroy_acceleration_structure(VkDevice device, VkAccelerationStructureKHR acc_struct)
{
    CVK_ASSERT(device != nullptr);
    CVK_ASSERT(acc_struct != nullptr);
    CVK_IMPORT_FUNCTION(device, vkDestroyAccelerationStructureKHR);
    vkDestroyAccelerationStructureKHR(device, acc_struct, nullptr);
}

CVK_API void get_acc_struct_address(VkDevice device, VkAccelerationStructureKHR acc_struct, uint64_t& addr)
{
    CVK_ASSERT(device != nullptr);
    CVK_IMPORT_FUNCTION(device, vkGetAccelerationStructureDeviceAddressKHR);
    VkAccelerationStructureDeviceAddressInfoKHR info {};
    info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
    info.accelerationStructure = acc_struct;
    addr = vkGetAccelerationStructureDeviceAddressKHR(device, &info);
}

} // namespace __cvk
