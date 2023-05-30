#pragma once

#include "cvk/vk_header.h"

namespace cvk
{

    struct CVK_API AccStructGeometryDataTriangles : public VkAccelerationStructureGeometryTrianglesDataKHR
    {
        constexpr AccStructGeometryDataTriangles() noexcept;

        inline constexpr void set_vertex_device_address(VkDeviceAddress addr) noexcept { vertexData.deviceAddress = addr; }
        inline constexpr void set_vertex_format(VkFormat format) noexcept { vertexFormat = format; }
        inline constexpr void set_vertex_max_num(uint32_t max_num) noexcept { maxVertex = max_num; }
        inline constexpr void set_vertex_stride(uint32_t stride) noexcept { vertexStride = stride; }

        inline constexpr void set_index_device_address(VkDeviceAddress addr) noexcept { indexData.deviceAddress = addr; }
        inline constexpr void set_index_type(VkIndexType type) noexcept { indexType = type; }

        inline constexpr void set_transform_device_address(VkDeviceAddress addr) noexcept { transformData.deviceAddress = addr; }
    };

    struct CVK_API AccStructGeometryDataInstances : public VkAccelerationStructureGeometryInstancesDataKHR
    {
        constexpr AccStructGeometryDataInstances() noexcept;

        inline auto& address() noexcept { return data; }
        inline auto CONST_REFERENCE address() const noexcept { return data; }

        inline constexpr void set_device_address(VkDeviceAddress addr) noexcept { data.deviceAddress = addr; }
        // inline constexpr void set_host_address(void CONST_PTR addr) noexcept { data.hostAddress = addr; }

    };
    
} // namespace cvk
