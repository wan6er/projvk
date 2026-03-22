#pragma once

#include "cvk/descriptor.h"
#include "cvk/buffer.h"
#include "cvk/pipe/pipeline_layout.h"
#include "load_model.h"
#include "load_texture.h"

inline void create_descriptor_layout(cvk::DescriptorSetLayout& descriptor_layout)
{
    descriptor_layout
            .set(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1)
            .set(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1)
            .set(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
            .create();
}

inline void create_pipeline_layout(cvk::PipelineLayout& layout, VkDescriptorSetLayout descriptor_layout)
{
    layout.attaches(descriptor_layout);
    CVK_ASSERT(layout.create() == VK_SUCCESS);
}

class Object
{
public:
    explicit Object(cvk::Device CONST_REFERENCE device) :
        _device(device),
        _sampler(device),
        _texture(device),
        _uniform(device),
        _descriptor_layout(device),
        _descriptor(device),
        _layout(device)
    {
    }

    void load(uint32_t graphics_index)
    {
        load_texture(_device, "texture/particle_gradient_rgba.ktx", graphics_index, _texture);
        _sampler.create();
        load_vertex(_device, "model/glowsphere.gltf", _obj_buffers);

        create_descriptor_layout(_descriptor_layout);
        create_descriptor(_descriptor_layout, _obj_buffers.size());
        create_pipeline_layout(_layout, _descriptor_layout);
    }

    void create_uniform(void* data_ptr, size_t size)
    {
        _uniform.create(_device.get_memory_properties(), size);
        _uniform.upload(data_ptr, size); 
    }

    void write_descriptor()
    {
        for (uint32_t i = 0; i < _obj_buffers.size(); ++i) {
            _descriptor[i].write(0, _uniform.get_descriptor_info());
            _descriptor[i].write(1, _obj_buffers[i].transform.get_descriptor_info());
            _descriptor[i].write(2, _texture.get_descriptor_info(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, _sampler));
        }
    }

    void record_draw_command(cvk::BaseCommand CONST_REFERENCE command_buffer)
    {
        for (uint32_t i = 0; i < _obj_buffers.size(); ++i) 
        {
            command_buffer.bind_descriptor_sets(VK_PIPELINE_BIND_POINT_GRAPHICS, _layout, { _descriptor[i] });
            command_buffer.bind_vertex_buffer(_obj_buffers[i].vertex);
            command_buffer.bind_index_buffer(VK_INDEX_TYPE_UINT16, _obj_buffers[i].index);
            command_buffer.draw_indexed(_obj_buffers[i].index.get_memory_size() / sizeof(uint16_t));
        }
    }

    inline VkPipelineLayout get_layout() const
    {
        return _layout;
    }

    inline cvk::MemoryBuffer& get_uniform() { return _uniform; }

protected:

    void create_descriptor(cvk::DescriptorSetLayout CONST_REFERENCE layout, uint32_t num_of_descriptor)
    {
        for (uint32_t i = 0; i < num_of_descriptor; ++i) {
            _descriptor.add_layout(layout);
        }
        CVK_ASSERT(_descriptor.create() == VK_SUCCESS);
    }

private:
    cvk::Device CONST_REFERENCE _device;

    cvk::Sampler _sampler;
    cvk::Texture2D _texture;
    cvk::UniformBufferWritable _uniform;
    std::vector<NodeBuffers> _obj_buffers;

    cvk::DescriptorSetLayout _descriptor_layout;
    cvk::Descriptor _descriptor;

    cvk::PipelineLayout _layout;

};
