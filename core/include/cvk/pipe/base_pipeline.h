#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"

namespace cvk
{
    
    class CVK_API BasePipeline :
        public utils::BaseObj<VkPipeline>
    {
    public:
        explicit BasePipeline(VkDevice device);
        BasePipeline(BasePipeline CONST_REFERENCE) = default;
        ~BasePipeline();

        // virtual VkResult create_cache() final;
        virtual operator VkPipeline() const final;

    protected:
        virtual void release() final;

        VkDevice device = VK_NULL_HANDLE;
        VkPipelineCache cache = VK_NULL_HANDLE;

    };

};