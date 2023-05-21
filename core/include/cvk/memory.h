#pragma once

#include "cvk/vk_header.h"
#include "cvk/memory/memory.h"
#include "cvk/memory/memory_ext.h"

namespace cvk
{

    using MemoryAddress = BaseTypeMemoryExt<Memory, VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT, 0>;

};
