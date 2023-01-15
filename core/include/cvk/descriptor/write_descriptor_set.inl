#pragma once

namespace cvk
{

template<class...__Args>
void WriteDescriptorSet::attaches(__Args&&...args)
{
    BaseAttachesType::attaches(std::forward<__Args>(args)...);
    
    std::vector<VkDescriptorImageInfo> CONST_REFERENCE image_infos = *this;
    std::vector<VkDescriptorBufferInfo> CONST_REFERENCE buffer_infos = *this;
    
    _write.descriptorCount = static_cast<uint32_t>(image_infos.size() + buffer_infos.size());
    _write.pImageInfo = image_infos.size() == 0 ? nullptr : image_infos.data();
    _write.pBufferInfo = buffer_infos.size() == 0 ? nullptr : buffer_infos.data();
}

};