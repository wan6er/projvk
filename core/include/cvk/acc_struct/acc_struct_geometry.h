#pragma once

#include "cvk/vk_header.h"
#include "cvk/acc_struct/acc_struct_geometry_data.h"

#include "utils/base_multi_attach.h"

namespace cvk
{    
    
    struct CVK_API AccStructGeometry : public VkAccelerationStructureGeometryKHR
    {
        constexpr AccStructGeometry() noexcept 
            : VkAccelerationStructureGeometryKHR()
        {
            sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
            flags = VK_GEOMETRY_OPAQUE_BIT_KHR;
        }

        auto instances() -> AccStructGeometryDataInstances&;
        auto instances() const -> AccStructGeometryDataInstances CONST_REFERENCE;

        auto triangles() -> AccStructGeometryDataTriangles&;
        auto triangles() const -> AccStructGeometryDataTriangles CONST_REFERENCE;

    }; 

    class CVK_API AccStructGeometries : public utils::BaseMultipleAttaches<VkAccelerationStructureGeometryKHR>
    {
    public:
        using _GeometriesAttaches = utils::BaseMultipleAttaches<VkAccelerationStructureGeometryKHR>;
        constexpr AccStructGeometries() noexcept 
            : _GeometriesAttaches()
        {
            _info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
            _info.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
        }

        template<class _Ty>
        void attach(_Ty&& geometry) { _GeometriesAttaches::attaches(std::forward<VkAccelerationStructureGeometryKHR>(geometry)); }

        constexpr inline void set_geometries_type(VkAccelerationStructureTypeKHR type) noexcept { _info.type = type; };

        auto get_geometries_info() const -> VkAccelerationStructureBuildGeometryInfoKHR CONST_REFERENCE;
        auto get_geometries_info() -> VkAccelerationStructureBuildGeometryInfoKHR&;
        auto get_geometries_size(VkDevice device, VkAccelerationStructureBuildTypeKHR type) -> VkAccelerationStructureBuildSizesInfoKHR;

    protected:
        using _GeometriesAttaches::attaches;

    private:
        VkAccelerationStructureBuildGeometryInfoKHR _info {}; 

    };
    
} // namespace cvk
