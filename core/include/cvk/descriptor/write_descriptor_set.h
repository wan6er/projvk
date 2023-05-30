#pragma once

#include "cvk/vk_header.h"
#include "utils/base_obj.h"
#include "utils/base_multi_attach.h"
#include "utils/base_single_attach.h"

namespace cvk
{

    class CVK_API WriteDescriptorSet :
        protected utils::BaseMultipleAttaches<
            VkDescriptorImageInfo, VkDescriptorBufferInfo,
            VkAccelerationStructureKHR>
    {
    protected:
        using BaseAttachesType = utils::BaseMultipleAttaches<VkDescriptorImageInfo, VkDescriptorBufferInfo, VkAccelerationStructureKHR>;

    public:
        WriteDescriptorSet();
        virtual ~WriteDescriptorSet() = default;

        template<class...__Args>
        auto attaches(__Args&&...args) -> WriteDescriptorSet&;
        auto setup(VkDescriptorSet set, VkDescriptorType type, uint32_t binding) -> WriteDescriptorSet&;
        auto setup(VkDescriptorSet set, VkDescriptorSetLayoutBinding CONST_REFERENCE layout_binding) -> WriteDescriptorSet&;
        void update(VkDevice device) const;

        operator VkWriteDescriptorSet CONST_REFERENCE () const;

    private:
        friend struct _WriteInfos;

        VkWriteDescriptorSet _write = {};
        VkWriteDescriptorSetAccelerationStructureKHR _write_accstruct = {};
    };

};

#include "write_descriptor_set.inl"