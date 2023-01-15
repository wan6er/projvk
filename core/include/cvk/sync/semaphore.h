#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"
#include "base_sync.h"

namespace cvk
{

    class CVK_API Semaphore :
        public BaseSync,
        public utils::BaseObj<VkSemaphore>
    {
    public:
        Semaphore(VkDevice device, bool signaled = false);
        virtual ~Semaphore();

        VkResult create();
        // VkResult wait(uint32_t timeout = UINT32_MAX);
        void set_create_signal(bool signaled);

        operator VkSemaphore CONST_REFERENCE () const;

    protected:
        void release();

    private:
        VkDevice _device;
        bool _signaled;

    };

};