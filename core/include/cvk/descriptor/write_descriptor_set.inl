#pragma once

#include "cvk/initialize/descriptor_initialize.h"

namespace cvk
{

template<class...__Args>
auto WriteDescriptorSet::attaches(__Args&&...args) -> WriteDescriptorSet&
{
    BaseAttachesType::attaches(std::forward<__Args>(args)...);
    
    // std::vector<VkDescriptorImageInfo> CONST_REFERENCE image_infos = *this;
    // std::vector<VkDescriptorBufferInfo> CONST_REFERENCE buffer_infos = *this;
    
    __cvk::get_write_descriptor_set_info(*this, *this, _write);
    return *this;
}

};