#include "cvk/pipe/descriptor.h"

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

DescriptorSetLayout& Descriptor::get_layout(size_t index)
{
    std::vector<DescriptorSetLayoutCallback>& layouts = *this;
    return layouts[index];
}

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

DescriptorSet& Descriptor::operator[](size_t set)
{
    std::vector<DescriptorSet>& sets = *this;
    return sets[set];
}

DescriptorSet CONST_REFERENCE Descriptor::operator[](size_t set) const
{
    std::vector<DescriptorSet> CONST_REFERENCE sets = *this;
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
    std::vector<DescriptorSet>& sets = *this;
    std::vector<DescriptorSetLayoutCallback>& layouts = *this;
    CVK_ASSERT(layouts.size() > 0);
    for (auto& layout : layouts) {
        result = layout.create();
        if (result != VK_SUCCESS) {
            break;
        }
        result = sets.emplace_back(get_device(), object(), layout).allocate();
        if (result != VK_SUCCESS) {
            break;
        }
    }
    return result;
}

} // namespace cvk
