#pragma once

#include "cvk/vk_header.h"
#include "write_descriptor_set.h"
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

        template<class _Ty>
        inline void write(uint32_t binding, _Ty&& info);

    protected:
        void free();

    private:
        VkDescriptorSetAllocateInfo _alloc_info;
        VkDevice _device = VK_NULL_HANDLE;

    };
};

namespace cvk
{

    template<class _Ty>
    inline void DescriptorSet::write(uint32_t binding, _Ty&& info)
    {
        CVK_ASSERT((VkDescriptorSet)*this != VK_NULL_HANDLE);
        cvk::WriteDescriptorSet write_set;
        write_set.attaches(info);
        write_set.setup(*this, get_layout_binding(binding));
        write_set.update(_device);
    }

};

#include "descriptor_set.inl"