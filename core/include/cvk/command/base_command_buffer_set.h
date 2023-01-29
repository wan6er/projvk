#pragma once

#include "cvk/vk_header.h"
#include "utils/base_objs.h"

namespace cvk
{
    
    template<class _DerivedType, VkCommandBufferLevel _Level>
    class BaseCommandBufferSet :
        public utils::BaseObjs<VkCommandBuffer>
    {
    public:
        BaseCommandBufferSet(VkDevice device, VkCommandPool pool, size_t size);
        virtual ~BaseCommandBufferSet();

        VkResult create();

        auto operator[](size_t size) const -> _DerivedType;
        // auto operator[](size_t size) -> _DerivedType&;

    protected:
        VkCommandBufferAllocateInfo alloc_info = {};
        void release();
        
    private:
        VkDevice _device;
        VkCommandPool _pool;

    };
    

} // namespace cvk
