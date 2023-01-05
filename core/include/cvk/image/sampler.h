#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"

namespace cvk
{
    class CVK_API Sampler :
        protected utils::BaseObj<VkSampler>
    {
    public:
        Sampler();
        virtual ~Sampler();

        VkResult create(VkDevice device);

        operator VkSampler CONST_REFERENCE () const;

    protected:
        void release();

    private:
        VkSamplerCreateInfo _create_info;
        VkDevice _device = VK_NULL_HANDLE;

    };
};