#pragma once

#include "cvk/vk_header.h"

namespace __cvk
{

    CVK_API void get_default_acc_struct_build_geometry_info(VkAccelerationStructureTypeKHR type, VkFlags flags, std::vector<VkAccelerationStructureGeometryKHR> CONST_REFERENCE geometries, VkAccelerationStructureBuildGeometryInfoKHR& info);
    CVK_API void get_acc_struct_geometries_size(VkDevice device, VkAccelerationStructureBuildTypeKHR type, VkAccelerationStructureBuildGeometryInfoKHR CONST_REFERENCE geometries, VkAccelerationStructureBuildSizesInfoKHR& size);

    CVK_API void get_default_acc_struct_create_info(VkAccelerationStructureTypeKHR type, VkBuffer buffer, uint64_t size, VkAccelerationStructureCreateInfoKHR& info);
    CVK_API VkResult create_acceleration_structure(VkDevice device, VkAccelerationStructureCreateInfoKHR CONST_REFERENCE info, VkAccelerationStructureKHR& acc_struct);
    CVK_API void destroy_acceleration_structure(VkDevice device, VkAccelerationStructureKHR acc_struct);
    CVK_API void get_acc_struct_address(VkDevice device, VkAccelerationStructureKHR acc_struct, uint64_t& addr);

    
} // namespace __cvk
