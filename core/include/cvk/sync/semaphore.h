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
        Semaphore(bool signaled = false);
        virtual ~Semaphore();

        VkResult create(VkDevice device);
        // VkResult wait(uint32_t timeout = UINT32_MAX);

        operator VkSemaphore CONST_REFERENCE () const;

    protected:
        void release();

    private:
        bool _signaled;
        VkDevice _device = VK_NULL_HANDLE;

    };

};