
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