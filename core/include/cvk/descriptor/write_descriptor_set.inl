#pragma once

#include "cvk/initialize/descriptor_initialize.h"

namespace cvk
{

template<class...__Args>
auto WriteDescriptorSet::attaches(__Args&&...args) -> WriteDescriptorSet&
{
    BaseAttachesType::attaches(std::forward<__Args>(args)...);
    return *this;
}

};