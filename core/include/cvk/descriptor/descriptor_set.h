#pragma once

#include "cvk/vk_header.h"
#include "write_descriptor_set.h"
#include "utils/base_obj.h"
#include "utils/base_multi_attach.h"
#include "utils/base_single_attach.h"

namespace cvk
{

    class CVK_API DescriptorSet :
        protected utils::BaseObj<VkDescriptorSet>
        // public utils::BaseMultipleAttaches<VkWriteDescriptorSet, WriteDescriptorSet>
    {
    public:
        DescriptorSet(VkDevice device, VkDescriptorPool pool, VkDescriptorSetLayout CONST_REFERENCE layout);
        virtual ~DescriptorSet();

        operator VkDescriptorSet CONST_REFERENCE () const;

        VkResult allocate();
        // void update();

    protected:
        void free();

    private:
        VkDescriptorSetAllocateInfo _alloc_info;
        VkDevice _device = VK_NULL_HANDLE;

    };
};