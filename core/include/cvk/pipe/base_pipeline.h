#pragma once

#include "cvk/vk_header.h"
#include "utils/base_device_obj.h"

namespace cvk
{
    
    class CVK_API BasePipeline :
        public utils::BaseDeviceObj<VkPipeline>
    {
        using _BaseDevice = utils::BaseDeviceObj<VkPipeline>;

    public:
        explicit BasePipeline(VkDevice device);
        BasePipeline(BasePipeline CONST_REFERENCE) = default;
        ~BasePipeline();

        inline auto cache() const noexcept { return _cache; }
        virtual operator VkPipeline() const final;

    protected:
        virtual void release() final;

        // VkDevice device = VK_NULL_HANDLE;
        VkPipelineCache _cache = VK_NULL_HANDLE;

    };

};