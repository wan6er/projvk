#pragma once

#include "cvk/buffer/memory_buffer.h"
#include "vk_header.h"

namespace cvk
{

    // typedef BaseMemorized<Buffer> MemorizedBuffer;

    typedef BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_STANDARD, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT> UniformBuffer;
    typedef BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_WRITABLE, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT> UniformBufferWritable;
    typedef BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_WRITABLE_SPEED, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT> UniformBufferWritableSpeed;
    
    typedef BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_STANDARD, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT> VertexBuffer;
    typedef BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_WRITABLE, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT> VertexBufferWritable;
    typedef BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_WRITABLE_SPEED, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT> VertexBufferWritableSpeed;

    typedef BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_STANDARD, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT> IndexBuffer;
    typedef BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_WRITABLE, VK_BUFFER_USAGE_INDEX_BUFFER_BIT> IndexBufferWritable;
    typedef BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_WRITABLE_SPEED, VK_BUFFER_USAGE_INDEX_BUFFER_BIT> IndexBufferWritableSpeed;

    typedef BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_WRITABLE, VK_BUFFER_USAGE_TRANSFER_SRC_BIT> BufferTransferSrc;
    typedef BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_WRITABLE_SPEED, VK_BUFFER_USAGE_TRANSFER_SRC_BIT> BufferTransferSrcSpeed;

    typedef BaseTypeMemoryBuffer<Buffer, MemoryAddress, MEMORY_STANDARD, VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR> AccelerationStructureInputBuffer;
    typedef BaseTypeMemoryBuffer<Buffer, MemoryAddress, MEMORY_STANDARD, VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR> AccelerationStructureInputBufferWritable;

    typedef BaseTypeMemoryBuffer<Buffer, MemoryAddress, MEMORY_WRITABLE, VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR> AccelerationStructureBuffer;
};