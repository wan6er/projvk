#pragma once

#include "cvk/vk_header.h"
// #include "write_descriptor_set.h"
#include "descriptor_set_layout.h"
#include "utils/base_obj.h"
#include "utils/base_multi_attach.h"
#include "utils/base_single_attach.h"

namespace cvk
{

    class CVK_API DescriptorSet :
        public DescriptorSetLayout,
        protected utils::BaseObj<VkDescriptorSet>
        // public utils::BaseMultipleAttaches<VkWriteDescriptorSet, WriteDescriptorSet>
    {
    public:
        typedef utils::BaseObj<VkDescriptorSet> SetType;

        DescriptorSet(VkDevice device, VkDescriptorPool pool);
        DescriptorSet(VkDevice device);
        DescriptorSet(VkDevice device, DescriptorSetLayout CONST_REFERENCE layout);
        virtual ~DescriptorSet();

        operator VkDescriptorSet CONST_REFERENCE () const;

        virtual VkResult create();
        virtual VkResult create(VkDescriptorPool pool);

        void write(uint32_t binding, VkDescriptorImageInfo CONST_REFERENCE info);
        void write(uint32_t binding, VkDescriptorBufferInfo CONST_REFERENCE info);
        // void update();

    protected:
        void free();

    private:
        VkDescriptorSetAllocateInfo _alloc_info;
        VkDevice _device = VK_NULL_HANDLE;

    };
};