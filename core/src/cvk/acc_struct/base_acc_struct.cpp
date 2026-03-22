#include "cvk/acc_struct/base_acc_struct.h"

#include "cvk/initialize/acc_struct_initialize.h"

namespace cvk
{
    
    AccelerationStructure::AccelerationStructure(VkDevice device) :
        _Base(), _Geometries(), _device(device)
    {
    }

    AccelerationStructure::~AccelerationStructure()
    {
        if (!is_cloned() && isolated()) {
            release();
        }
    }
        
    VkResult AccelerationStructure::create(VkAccelerationStructureTypeKHR type, VkBuffer buffer, uint32_t size)
    {
        __cvk::get_default_acc_struct_create_info(type, buffer, size, _info);
        return __cvk::create_acceleration_structure(_device, _info, object());
    }
        
    uint64_t AccelerationStructure::get_address()
    {
        uint64_t addr = 0;
        __cvk::get_acc_struct_address(_device, object(), addr);
        return addr;
    }
        
    void AccelerationStructure::release()
    {
        __cvk::destroy_acceleration_structure(_device, object());
    }

} // namespace cvk
