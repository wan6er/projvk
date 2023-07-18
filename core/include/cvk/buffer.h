#pragma once

#include "cvk/buffer/memory_buffer.h"
#include "vk_header.h"

namespace cvk
{

    // using MemorizedBuffer = BaseMemorized<Buffer>;

    using UniformBuffer = BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_STANDARD, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT>;
    using UniformBufferWritable = BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_WRITABLE, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT>;
    using UniformBufferWritableSpeed = BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_WRITABLE_SPEED, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT>;
    
    using VertexBuffer = BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_STANDARD, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT>;
    using VertexBufferWritable = BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_WRITABLE, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT>;
    using VertexBufferWritableSpeed = BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_WRITABLE_SPEED, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT>;

    using IndexBuffer = BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_STANDARD, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT>;
    using IndexBufferWritable = BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_WRITABLE, VK_BUFFER_USAGE_INDEX_BUFFER_BIT>;
    using IndexBufferWritableSpeed = BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_WRITABLE_SPEED, VK_BUFFER_USAGE_INDEX_BUFFER_BIT>;

    using BufferTransferSrc = BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_WRITABLE, VK_BUFFER_USAGE_TRANSFER_SRC_BIT>;
    using BufferTransferSrcSpeed = BaseTypeMemoryBuffer<Buffer, Memory, MEMORY_WRITABLE_SPEED, VK_BUFFER_USAGE_TRANSFER_SRC_BIT>;

};

#ifdef CVK_RAYTRACING

#include "cvk/buffer/memory_address_buffer.h"

namespace cvk
{
    
    using AccelerationStructureInputBuffer = BaseTypeMemoryAddressBuffer<Buffer, MemoryAddress, MEMORY_STANDARD, VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR>;
    using AccelerationStructureInputBufferWritable = BaseTypeMemoryAddressBuffer<Buffer, MemoryAddress, MEMORY_WRITABLE, VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR>;

    using AccelerationStructureBuffer = BaseTypeMemoryAddressBuffer<Buffer, MemoryAddress, MEMORY_STANDARD, VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR>;
    using AccelerationStructureScratchBuffer = BaseTypeMemoryAddressBuffer<Buffer, MemoryAddress, MEMORY_STANDARD, VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT>;

    using ShaderBindingTableBufferWritable = BaseTypeMemoryAddressBuffer<Buffer, MemoryAddress, MEMORY_WRITABLE, VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR>;

} // namespace cvk

#endif