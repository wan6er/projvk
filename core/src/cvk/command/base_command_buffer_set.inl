#include "cvk/command/base_command_buffer_set.h"
#include "cvk/initialize/command_initialize.h"

namespace cvk
{
    
template<class _DerivedType, VkCommandBufferLevel _Level>
BaseCommandBufferSet<_DerivedType, _Level>::BaseCommandBufferSet(VkDevice device, VkCommandPool pool, size_t size) :
    _device(device),
    _pool(pool)
{
    __cvk::get_default_command_buffers_allocate_info(_pool, _Level, size, alloc_info);
}

template<class _DerivedType, VkCommandBufferLevel _Level>
BaseCommandBufferSet<_DerivedType, _Level>::~BaseCommandBufferSet()
{
    if (isolated() && !is_cloned()) {
        release();
    }
}

template<class _DerivedType, VkCommandBufferLevel _Level>
void BaseCommandBufferSet<_DerivedType, _Level>::release()
{
    if (_device != VK_NULL_HANDLE && _pool != VK_NULL_HANDLE && objects().size() > 0) {
        __cvk::free_command_buffers(_device, _pool, objects());
    }
}

template<class _DerivedType, VkCommandBufferLevel _Level>
VkResult BaseCommandBufferSet<_DerivedType, _Level>::create()
{
    objects().resize(alloc_info.commandBufferCount);
    return __cvk::alloc_command_buffers(_device, alloc_info, objects());
}


template<class _DerivedType, VkCommandBufferLevel _Level>
auto BaseCommandBufferSet<_DerivedType, _Level>::operator[](size_t size) const -> _DerivedType
{
    return _DerivedType(objects()[size]);
}

}