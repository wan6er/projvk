#pragma once

#include "cvk/base/base_memorized.h"
#include "cvk/base/base_memorized_set.h"
#include "cvk/buffer.h"

namespace cvk
{
    
    typedef BaseMemorized<Buffer> MemorizedBuffer;

    typedef BaseTypeMemorized<Buffer, MEMORY_STANDARD> StandardBuffer;
    typedef BaseTypeMemorized<Buffer, MEMORY_WRITABLE> WritableBuffer;
    typedef BaseTypeMemorized<Buffer, MEMORY_READABLE> ReadableBuffer;
    typedef BaseTypeMemorized<Buffer, MEMORY_WRITABLE_FULL_SPEED> WritableBufferFullSpeed;
    
    typedef BaseTypeMemorized<BaseTypeBuffer<VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT>, MEMORY_STANDARD> StandardUniformBuffer;
    typedef BaseTypeMemorized<BaseTypeBuffer<VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT>, MEMORY_WRITABLE> WritableUniformBuffer;
    typedef BaseTypeMemorized<BaseTypeBuffer<VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT>, MEMORY_WRITABLE_FULL_SPEED> WritableUniformBufferFullSpeed;
    
    typedef BaseTypeMemorized<BaseTypeBuffer<VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT>, MEMORY_STANDARD> StandardVertexBuffer;
    typedef BaseTypeMemorized<BaseTypeBuffer<VK_BUFFER_USAGE_VERTEX_BUFFER_BIT>, MEMORY_WRITABLE> WritableVertexBuffer;
    typedef BaseTypeMemorized<BaseTypeBuffer<VK_BUFFER_USAGE_VERTEX_BUFFER_BIT>, MEMORY_WRITABLE_FULL_SPEED> WritableVertexBufferFullSpeed;

    typedef BaseTypeMemorized<BaseTypeBuffer<VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT>, MEMORY_STANDARD> StandardIndexBuffer;
    typedef BaseTypeMemorized<BaseTypeBuffer<VK_BUFFER_USAGE_INDEX_BUFFER_BIT>, MEMORY_WRITABLE> WritableIndexBuffer;
    typedef BaseTypeMemorized<BaseTypeBuffer<VK_BUFFER_USAGE_INDEX_BUFFER_BIT>, MEMORY_WRITABLE_FULL_SPEED> WritableIndexBufferFullSpeed;

    typedef BaseTypeMemorized<BaseTypeBuffer<VK_BUFFER_USAGE_TRANSFER_SRC_BIT>, MEMORY_WRITABLE> TransferSrcBuffer;
    typedef BaseTypeMemorized<BaseTypeBuffer<VK_BUFFER_USAGE_TRANSFER_SRC_BIT>, MEMORY_WRITABLE_FULL_SPEED> TransferSrcBufferFullSpeed;

} // namespace cvk
