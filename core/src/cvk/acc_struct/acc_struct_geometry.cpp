#include "cvk/acc_struct/acc_struct_geometry.h"

#include "cvk/initialize/acc_struct_initialize.h"

#include "utils/vector_util.h"

namespace cvk
{

    constexpr AccStructGeometry::AccStructGeometry() noexcept :
        VkAccelerationStructureGeometryKHR()
    {
        sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
        flags = VK_GEOMETRY_OPAQUE_BIT_KHR;
    }

    constexpr auto AccStructGeometry::instances() -> AccStructGeometryDataInstances&
    {
        geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
        geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
        return static_cast<AccStructGeometryDataInstances&>(geometry.instances);
    }
    
    constexpr auto AccStructGeometry::instances() const -> AccStructGeometryDataInstances CONST_REFERENCE
    {
        return static_cast<AccStructGeometryDataInstances CONST_REFERENCE>(geometry.instances);
    }

    constexpr auto AccStructGeometry::triangles() -> AccStructGeometryDataTriangles&
    {
        geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
        geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
        return static_cast<AccStructGeometryDataTriangles&>(geometry.triangles);
    }
    
    constexpr auto AccStructGeometry::triangles() const -> AccStructGeometryDataTriangles CONST_REFERENCE
    {
        return static_cast<AccStructGeometryDataTriangles CONST_REFERENCE>(geometry.triangles);
    }



    constexpr AccStructGeometries::AccStructGeometries() noexcept : 
        _GeometriesAttaches()
    {
        _info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
        _info.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
    }

    auto AccStructGeometries::get_geometries_info() const -> VkAccelerationStructureBuildGeometryInfoKHR CONST_REFERENCE
    {
        // utils::vector_fill_info(_GeometriesAttaches::get_attachments(), _info.geometryCount, _info.pGeometries);
        return _info;
    }

    auto AccStructGeometries::get_geometries_info() -> VkAccelerationStructureBuildGeometryInfoKHR&
    {
        utils::vector_fill_info(_GeometriesAttaches::get_attachments(), _info.geometryCount, _info.pGeometries);
        return _info;
    }

    auto AccStructGeometries::get_geometries_size(VkDevice device, VkAccelerationStructureBuildTypeKHR type) -> VkAccelerationStructureBuildSizesInfoKHR
    {
        VkAccelerationStructureBuildSizesInfoKHR size {};
        __cvk::get_acc_struct_geometries_size(device, type, get_geometries_info(), size);
        return size;
    }

} // namespace cvk
