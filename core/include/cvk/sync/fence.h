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
        Fence(VkDevice device, bool signaled = false);
        virtual ~Fence();

        VkResult create();
        VkResult wait(uint32_t timeout = UINT32_MAX);
        VkResult reset();
        void set_create_signal(bool signaled);

        operator VkFence CONST_REFERENCE () const;

    protected:
        void release();

    private:
        VkDevice _device;
        bool _signaled;

    };

};