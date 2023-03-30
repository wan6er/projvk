#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "cgltf/gltf_node.h"

#include "cvk/device.h"
#include "cvk/image.h"
#include "cvk/buffer.h"
#include "cvk/fence.h"
#include "cvk/command_pool.h"
#include "cvk/command_buffer.h"

#include "utils/vertex_data.h"
#include "utils/tuple.h"

struct NodeBuffers
{
    cvk::WritableVertexBuffer vertex;
    cvk::WritableIndexBuffer index;
    cvk::WritableUniformBuffer transform;

    NodeBuffers(VkDevice device) :
        vertex(device), index(device), transform(device)
    {
    }
};

// typedef utils::TupleUnion<NodeBuffersImpl, cvk::WritableVertexBuffer, cvk::WritableIndexBuffer, cvk::WritableUniformBuffer> NodeBuffers;

struct Vertex
{
    glm::vec3 POSITION;
    glm::vec3 NORMAL;
    glm::vec3 COLOR_0;
    glm::vec2 TEXCOORD_0;

    Vertex() : COLOR_0(1.0f) {}
};

void load_vertex(cvk::Device CONST_REFERENCE device, std::string filename, std::vector<NodeBuffers>& node_buffers)
{
    // std::string filename = "../test/vk-test/models/samplebuilding.gltf";
    std::string error, warning;
    tinygltf::TinyGLTF gltfContext;
    tinygltf::Model gltfModel;
    // gltfContext.SetImageLoader(loadImageDataFunc, nullptr);
    gltfContext.LoadASCIIFromFile(&gltfModel, &error, &warning, filename);
    std::vector<cgltf::Node<Vertex, uint16_t>> nodes;
    uint32_t i = 0;
    for (auto& node : gltfModel.nodes) {
        if (node.mesh < 0) {
            continue;
        }
        // cgltf::Node<Vertex, uint16_t> node_data;
        nodes.emplace_back();
        nodes.back().setup_info(VERTEX_DATA_REG_MEMBERS(Vertex, POSITION, NORMAL, COLOR_0, TEXCOORD_0));
        nodes.back().load(gltfModel, node);
        nodes.back().load_material(gltfModel, node, "COLOR_0");
        i++;
    }

    node_buffers.reserve(nodes.size());
    for (auto& node : nodes) {
        node_buffers.emplace_back(device);

        CVK_ASSERT(node_buffers.back().vertex.create(device.get_memory_properties(), node.get_vertexes().get_data_size()) == VK_SUCCESS);
        CVK_ASSERT(node_buffers.back().vertex.upload(node.get_vertexes().get_data(), node.get_vertexes().get_data_size()) == VK_SUCCESS);

        CVK_ASSERT(node_buffers.back().index.create(device.get_memory_properties(), node.get_vertexes().get_data_size()) == VK_SUCCESS);
        CVK_ASSERT(node_buffers.back().index.upload(node.get_indices().data(), node.get_indices().size() * sizeof(uint16_t)) == VK_SUCCESS);

        CVK_ASSERT(node_buffers.back().transform.create(device.get_memory_properties(), sizeof(glm::mat4)) == VK_SUCCESS);
        auto transform_matrix = node.get_transform().to_matrix();
        CVK_ASSERT(node_buffers.back().transform.upload(&transform_matrix, sizeof(transform_matrix)) == VK_SUCCESS);
    }
}
