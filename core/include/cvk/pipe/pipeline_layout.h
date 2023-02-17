#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"
#include "utils/base_multi_attach.h"

namespace cvk
{
    class CVK_API PipelineLayout : 
        protected utils::BaseObj<VkPipelineLayout>,
        public utils::BaseMultipleAttaches<VkDescriptorSetLayout>
    {
    public:
        explicit PipelineLayout(VkDevice device);
        PipelineLayout(const PipelineLayout&) = default;
        ~PipelineLayout();

        VkResult create();

        operator VkPipelineLayout CONST_REFERENCE () const;
    
    protected:
        void release();

    private:
        VkPipelineLayoutCreateInfo _create_info;
        VkDevice _device = VK_NULL_HANDLE;

    };
};