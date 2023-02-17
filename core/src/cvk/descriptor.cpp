#include "cvk/descriptor.h"

namespace cvk
{
    
Descriptor::Descriptor(VkDevice device) :
    DescriptorPool(device)
{
}

Descriptor::~Descriptor()
{

}

DescriptorSetLayout& Descriptor::add_layout()
{
    std::vector<DescriptorSetLayoutCallback>& layouts = *this;
    return layouts.emplace_back(*this, get_device());
}

void Descriptor::add_layout(DescriptorSetLayout CONST_REFERENCE layout)
{
    std::vector<DescriptorSetLayoutCallback>& layouts = *this;
    layouts.emplace_back(*this, get_device(), layout);
}

// DescriptorSetLayout& Descriptor::get_layout(size_t index)
// {
//     std::vector<DescriptorSetLayoutCallback>& layouts = *this;
//     return layouts[index];
// }

VkResult Descriptor::create()
{
    CVK_ASSERT(get_device() != VK_NULL_HANDLE);
    VkResult result = VK_SUCCESS;

    result = create_pool();
    if (result != VK_SUCCESS) {
        goto __RESULT;
    }

    result = create_sets();
    if (result != VK_SUCCESS) {
        goto __RESULT;
    }

__RESULT:
    return result;
}

DescriptorSetLayoutCallback& Descriptor::operator[](size_t set)
{
    std::vector<DescriptorSetLayoutCallback>& sets = *this;
    return sets[set];
}

DescriptorSetLayoutCallback CONST_REFERENCE Descriptor::operator[](size_t set) const
{
    std::vector<DescriptorSetLayoutCallback> CONST_REFERENCE sets = *this;
    return sets[set];
}

VkResult Descriptor::create_pool()
{
    std::vector<DescriptorSetLayoutCallback> CONST_REFERENCE layouts = *this;
    CVK_ASSERT(layouts.size() > 0);
    return DescriptorPool::create(static_cast<uint32_t>(layouts.size()));
}

VkResult Descriptor::create_sets()
{
    VkResult result = VK_SUCCESS;
    // std::vector<DescriptorSet>& sets = *this;
    std::vector<DescriptorSetLayoutCallback>& layouts = *this;
    CVK_ASSERT(layouts.size() > 0);
    for (auto& layout : layouts) {
        result = layout.create(DescriptorPool::object());
        if (result != VK_SUCCESS) {
            break;
        }
    }
    return result;
}

} // namespace cvk
