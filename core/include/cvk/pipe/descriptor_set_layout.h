#pragma once 

#include "cvk/vk_header.h"
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
        explicit DescriptorSetLayout();
        DescriptorSetLayout(DescriptorSetLayout CONST_REFERENCE) = default;
        virtual ~DescriptorSetLayout();

        VkResult create(VkDevice device);

        VkDescriptorSetLayout CONST_REFERENCE get_layout() const;
        operator VkDescriptorSetLayout CONST_REFERENCE () const;

    protected:
        void release();

    private:
        std::vector<VkDescriptorSetLayoutBinding> _temp_bindings;
        VkDescriptorSetLayoutCreateInfo _create_info;
        VkDevice _device = VK_NULL_HANDLE;
    };
};