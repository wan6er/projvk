#pragma once 

#include "cvk/vk_header.h"
#include "descriptor_pool.h"
#include "utils/base_obj.h"
#include "utils/base_multi_attach.h"

namespace cvk
{
    struct CVK_API DescriptorSetLayoutBinding : public VkDescriptorSetLayoutBinding
    {
        DescriptorSetLayoutBinding() = default;
        DescriptorSetLayoutBinding(VkDescriptorType type, VkShaderStageFlags shader_stage, uint32_t binding, uint32_t num_of_descriptor = 1);

        // VkDescriptorPoolSize get_pool_size() const;
    };

    class CVK_API DescriptorSetLayout :
        protected utils::BaseObj<VkDescriptorSetLayout>,
        public utils::BaseMultipleAttaches<VkDescriptorSetLayoutBinding>
    {
    public:
        DescriptorSetLayout(VkDevice device);
        DescriptorSetLayout(DescriptorSetLayout CONST_REFERENCE) = default;
        virtual ~DescriptorSetLayout();

        virtual VkResult create();
        virtual DescriptorSetLayout& set(uint32_t binding, VkDescriptorType type, VkShaderStageFlags shader_stage, uint32_t num_of_descriptor);
        VkDescriptorSetLayoutBinding CONST_REFERENCE get_layout_binding(uint32_t binding) const;
        virtual DescriptorSetLayout copy() const;


        VkDescriptorSetLayout CONST_REFERENCE get_layout() const;
        operator VkDescriptorSetLayout CONST_REFERENCE () const;

    protected:
        void release();

    private:
        friend class DescriptorPool;

        std::vector<VkDescriptorSetLayoutBinding> _temp_bindings;
        VkDescriptorSetLayoutCreateInfo _create_info;
        VkDevice _device = VK_NULL_HANDLE;
    };
};