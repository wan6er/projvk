#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"
#include "utils/base_multi_attach.h"
#include "utils/base_single_attach.h"

namespace cvk
{
    class CVK_API WriteDescriptorSet :
        protected utils::BaseMultipleAttaches<VkDescriptorImageInfo, VkDescriptorBufferInfo>
    {
    public:
        // explicit WriteDescriptorSet();
        WriteDescriptorSet(VkDescriptorType type, uint32_t binding);
        WriteDescriptorSet(VkDescriptorSetLayoutBinding CONST_REFERENCE layout_binding);

        template<class...__Args>
        void attaches(__Args&&...args);
        void update(VkDevice device, VkDescriptorSet set);

        operator VkWriteDescriptorSet CONST_REFERENCE () const;

    protected:
        using BaseAttachesType = utils::BaseMultipleAttaches<VkDescriptorImageInfo, VkDescriptorBufferInfo>;

    private:
        VkWriteDescriptorSet _write = {};
    };

};

#include "write_descriptor_set.inl"