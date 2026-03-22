#pragma once

#include "cvk/acc_struct/acc_struct_geometry.h"

#include "utils/base_device_obj.h"

namespace cvk
{

    class CVK_API AccelerationStructure : 
        public utils::BaseObj<VkAccelerationStructureKHR>,
        public AccStructGeometries
    {
        using _Base = utils::BaseObj<VkAccelerationStructureKHR>;
        using _Geometries = AccStructGeometries;

    public:
        AccelerationStructure(VkDevice device);
        ~AccelerationStructure();

        inline operator VkAccelerationStructureKHR() const noexcept { return object(); }

        VkResult create(VkAccelerationStructureTypeKHR type, VkBuffer buffer, uint32_t size);
        uint64_t get_address();

    protected:
        void release();

    private:
        VkAccelerationStructureCreateInfoKHR _info {};
        VkDevice _device;

    };
    
} // namespace cvk
