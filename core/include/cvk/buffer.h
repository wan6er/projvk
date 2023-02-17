#pragma once

#include "cvk/buffer/memory_buffer.h"
#include "vk_header.h"

namespace cvk
{

    // typedef BaseMemorized<Buffer> MemorizedBuffer;

    typedef BaseTypeMemoryBuffer<MEMORY_STANDARD, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT> StandardUniformBuffer;
    typedef BaseTypeMemoryBuffer<MEMORY_WRITABLE, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT> WritableUniformBuffer;
    typedef BaseTypeMemoryBuffer<MEMORY_WRITABLE_FULL_SPEED, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT> WritableUniformBufferFullSpeed;
    
    typedef BaseTypeMemoryBuffer<MEMORY_STANDARD, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT> StandardVertexBuffer;
    typedef BaseTypeMemoryBuffer<MEMORY_WRITABLE, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT> WritableVertexBuffer;
    typedef BaseTypeMemoryBuffer<MEMORY_WRITABLE_FULL_SPEED, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT> WritableVertexBufferFullSpeed;

    typedef BaseTypeMemoryBuffer<MEMORY_STANDARD, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT> StandardIndexBuffer;
    typedef BaseTypeMemoryBuffer<MEMORY_WRITABLE, VK_BUFFER_USAGE_INDEX_BUFFER_BIT> WritableIndexBuffer;
    typedef BaseTypeMemoryBuffer<MEMORY_WRITABLE_FULL_SPEED, VK_BUFFER_USAGE_INDEX_BUFFER_BIT> WritableIndexBufferFullSpeed;

    typedef BaseTypeMemoryBuffer<MEMORY_WRITABLE, VK_BUFFER_USAGE_TRANSFER_SRC_BIT> TransferSrcBuffer;
    typedef BaseTypeMemoryBuffer<MEMORY_WRITABLE_FULL_SPEED, VK_BUFFER_USAGE_TRANSFER_SRC_BIT> TransferSrcBufferFullSpeed;

};