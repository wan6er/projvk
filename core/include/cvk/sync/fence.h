#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"
#include "base_sync.h"

namespace cvk
{

    class CVK_API Fence :
        public BaseSync,
        public utils::BaseObj<VkFence>
    {
    public:
        Fence(bool signaled = false);
        virtual ~Fence();

        VkResult create(VkDevice device);
        VkResult wait(uint32_t timeout = UINT32_MAX);

        operator VkFence CONST_REFERENCE () const;

    protected:
        void release();

    private:
        bool _signaled;
        VkDevice _device = VK_NULL_HANDLE;

    };

};