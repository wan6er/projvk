#include "cvk/acc_struct/acc_struct_geometry_data.h"

namespace cvk
{

    constexpr AccStructGeometryDataTriangles::AccStructGeometryDataTriangles() noexcept :
        VkAccelerationStructureGeometryTrianglesDataKHR({})
    {
        sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;

    }

    constexpr AccStructGeometryDataInstances::AccStructGeometryDataInstances() noexcept :
        VkAccelerationStructureGeometryInstancesDataKHR({})
    {
        sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;

    }

} // namespace cvk
