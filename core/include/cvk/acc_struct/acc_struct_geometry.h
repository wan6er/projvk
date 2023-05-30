#pragma once

#include "cvk/vk_header.h"
#include "cvk/acc_struct/acc_struct_geometry_data.h"

#include "utils/base_multi_attach.h"

namespace cvk
{    
    
    struct CVK_API AccStructGeometry : public VkAccelerationStructureGeometryKHR
    {
        constexpr AccStructGeometry() noexcept;

        constexpr auto instances() -> AccStructGeometryDataInstances&;
        constexpr auto instances() const -> AccStructGeometryDataInstances CONST_REFERENCE;

        constexpr auto triangles() -> AccStructGeometryDataTriangles&;
        constexpr auto triangles() const -> AccStructGeometryDataTriangles CONST_REFERENCE;

    }; 

    class CVK_API AccStructGeometries : public utils::BaseMultipleAttaches<VkAccelerationStructureGeometryKHR>
    {
        using _GeometriesAttaches = utils::BaseMultipleAttaches<VkAccelerationStructureGeometryKHR>;
    public:
        constexpr AccStructGeometries() noexcept;

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
