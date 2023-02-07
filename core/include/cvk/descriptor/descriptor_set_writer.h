#pragma once

#include "cvk/vk_header.h"
#include "utils/base_objs.h"
#include "write_descriptor_set.h"

namespace cvk
{
    class CVK_API DescriptorSetWriter :
        public utils::BaseObjs<WriteDescriptorSet>
    {
    public:
        DescriptorSetWriter() noexcept = default;
        explicit DescriptorSetWriter(size_t size);
        virtual ~DescriptorSetWriter() = default;

        std::vector<VkWriteDescriptorSet> get_infos() const;

        void update(VkDevice device);

    protected:
        using BaseAttachesType = utils::BaseMultipleAttaches<VkDescriptorImageInfo, VkDescriptorBufferInfo>;

    };

};

#include "write_descriptor_set.inl"