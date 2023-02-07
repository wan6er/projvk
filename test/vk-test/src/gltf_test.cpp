#include "test_header.h"
// #define TINYGLTF_IMPLEMENTATION
// #define STB_IMAGE_IMPLEMENTATION
// #define TINYGLTF_NO_STB_IMAGE_WRITE
#include "cgltf/gltf_node.h"
#include "utils/tuple.h"
#include "utils/vertex_data.h"
#include "utils/def_utils.h"
#include <tuple>
#include <glm/glm.hpp>
#include <string>

    struct Vertex {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 uv;
        glm::vec4 color;
        glm::vec4 joint0;
        glm::vec4 weight0;
        glm::vec4 tangent;
        static VkVertexInputBindingDescription vertexInputBindingDescription;
        static std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions;
        static VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
        // static VkVertexInputBindingDescription inputBindingDescription(uint32_t binding);
        // static VkVertexInputAttributeDescription inputAttributeDescription(uint32_t binding, uint32_t location, VertexComponent component);
        // static std::vector<VkVertexInputAttributeDescription> inputAttributeDescriptions(uint32_t binding, const std::vector<VertexComponent> components);
        // /** @brief Returns the default pipeline vertex input state create info structure for the requested vertex components */
        // static VkPipelineVertexInputStateCreateInfo* getPipelineVertexInputState(const std::vector<VertexComponent> components);
    };

TEST_FUNC_BEGIN("gltf")

    std::string filename = "../test/vk-test/models/samplebuilding.gltf";
    std::string error, warning;
    tinygltf::TinyGLTF gltfContext;
    tinygltf::Model model;
    // gltfContext.SetImageLoader(loadImageDataFunc, nullptr);
    gltfContext.LoadASCIIFromFile(&model, &error, &warning, filename);

    // struct Vertex
    // {
    //     glm::vec3 POSITION;
    //     glm::vec3 NORMAL;
    //     glm::vec2 TEXCOORD_0;
    // };
    // utils::VectexData<Vertex> vertexes(VERTEX_DATA_REG_MEMBERS(Vertex, POSITION, NORMAL, TEXCOORD_0));
    // std::vector<uint16_t> indices;
    // {

    //     for (auto& mesh : model.meshes) {
    //         for (auto& primitive : mesh.primitives) {
    //             uint16_t vertex_start = vertexes.get_size();
    //             auto& index_accessor = model.accessors[primitive.indices];
    //             auto& index_buffer_view = model.bufferViews[index_accessor.bufferView];
    //             auto index_buffer = reinterpret_cast<uint16_t*>(&model.buffers[index_buffer_view.buffer].data[index_accessor.byteOffset + index_buffer_view.byteOffset]);
    //             indices.reserve(indices.size() + index_accessor.count);
    //             for (uint32_t i = 0; i < index_accessor.count; ++i) {
    //                 indices.emplace_back(index_buffer[i] + vertex_start);
    //             }

    //             for (auto& attr : primitive.attributes) {
    //                 auto& accessor = model.accessors[attr.second];
    //                 auto& buffer_view = model.bufferViews[accessor.bufferView];
    //                 if (vertex_start + accessor.count > vertexes.get_size()) {
    //                     vertexes.resize(vertexes.get_size() + accessor.count);
    //                 }

    //                 for (uint32_t i = 0; i < accessor.count; ++i) {
    //                     vertexes.set(i + vertex_start, attr.first, &model.buffers[buffer_view.buffer].data[i * vertexes.get_info(attr.first).size + buffer_view.byteOffset + accessor.byteOffset]);
    //                 }
    //             }
    //         }
    //     }
    // }

    // {
    //     std::vector<cgltf::Node<Vertex, uint16_t>> nodes;
    //     for (auto& node : model.nodes) {
    //         if (node.mesh < 0) {
    //             continue;
    //         }
    //         // cgltf::Node<Vertex, uint16_t> node_data;
    //         nodes.emplace_back();
    //         nodes.back().setup_info(VERTEX_DATA_REG_MEMBERS(Vertex, POSITION, NORMAL, TEXCOORD_0));
    //         nodes.back().load(model, node);

    //     }
    // }
    std::vector<uint32_t> indexBuffer;
    std::vector<Vertex> vertexBuffer;
    for (auto& node : model.nodes) {
        if (!(node.mesh > -1)) {
            continue;
        }
        
        auto const& mesh = model.meshes[node.mesh];
        for (size_t j = 0; j < mesh.primitives.size(); j++) {
            const tinygltf::Primitive &primitive = mesh.primitives[j];
            if (primitive.indices < 0) {
                continue;
            }
            uint32_t indexStart = static_cast<uint32_t>(indexBuffer.size());
            uint32_t vertexStart = static_cast<uint32_t>(vertexBuffer.size());
            uint32_t indexCount = 0;
            uint32_t vertexCount = 0;
            glm::vec3 posMin{};
            glm::vec3 posMax{};
            bool hasSkin = false;
            // Vertices
            {
                const float *bufferPos = nullptr;
                const float *bufferNormals = nullptr;
                const float *bufferTexCoords = nullptr;
                const float* bufferColors = nullptr;
                const float *bufferTangents = nullptr;
                uint32_t numColorComponents;
                const uint16_t *bufferJoints = nullptr;
                const float *bufferWeights = nullptr;

                // Position attribute is required
                assert(primitive.attributes.find("POSITION") != primitive.attributes.end());

                const tinygltf::Accessor &posAccessor = model.accessors[primitive.attributes.find("POSITION")->second];
                const tinygltf::BufferView &posView = model.bufferViews[posAccessor.bufferView];
                bufferPos = reinterpret_cast<const float *>(&(model.buffers[posView.buffer].data[posAccessor.byteOffset + posView.byteOffset]));
                posMin = glm::vec3(posAccessor.minValues[0], posAccessor.minValues[1], posAccessor.minValues[2]);
                posMax = glm::vec3(posAccessor.maxValues[0], posAccessor.maxValues[1], posAccessor.maxValues[2]);

                if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
                    const tinygltf::Accessor &normAccessor = model.accessors[primitive.attributes.find("NORMAL")->second];
                    const tinygltf::BufferView &normView = model.bufferViews[normAccessor.bufferView];
                    bufferNormals = reinterpret_cast<const float *>(&(model.buffers[normView.buffer].data[normAccessor.byteOffset + normView.byteOffset]));
                }

                if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
                    const tinygltf::Accessor &uvAccessor = model.accessors[primitive.attributes.find("TEXCOORD_0")->second];
                    const tinygltf::BufferView &uvView = model.bufferViews[uvAccessor.bufferView];
                    bufferTexCoords = reinterpret_cast<const float *>(&(model.buffers[uvView.buffer].data[uvAccessor.byteOffset + uvView.byteOffset]));
                }

                if (primitive.attributes.find("COLOR_0") != primitive.attributes.end())
                {
                    const tinygltf::Accessor& colorAccessor = model.accessors[primitive.attributes.find("COLOR_0")->second];
                    const tinygltf::BufferView& colorView = model.bufferViews[colorAccessor.bufferView];
                    // Color buffer are either of type vec3 or vec4
                    numColorComponents = colorAccessor.type == TINYGLTF_PARAMETER_TYPE_FLOAT_VEC3 ? 3 : 4;
                    bufferColors = reinterpret_cast<const float*>(&(model.buffers[colorView.buffer].data[colorAccessor.byteOffset + colorView.byteOffset]));
                }

                if (primitive.attributes.find("TANGENT") != primitive.attributes.end())
                {
                    const tinygltf::Accessor &tangentAccessor = model.accessors[primitive.attributes.find("TANGENT")->second];
                    const tinygltf::BufferView &tangentView = model.bufferViews[tangentAccessor.bufferView];
                    bufferTangents = reinterpret_cast<const float *>(&(model.buffers[tangentView.buffer].data[tangentAccessor.byteOffset + tangentView.byteOffset]));
                }

                // Skinning
                // Joints
                if (primitive.attributes.find("JOINTS_0") != primitive.attributes.end()) {
                    const tinygltf::Accessor &jointAccessor = model.accessors[primitive.attributes.find("JOINTS_0")->second];
                    const tinygltf::BufferView &jointView = model.bufferViews[jointAccessor.bufferView];
                    bufferJoints = reinterpret_cast<const uint16_t *>(&(model.buffers[jointView.buffer].data[jointAccessor.byteOffset + jointView.byteOffset]));
                }

                if (primitive.attributes.find("WEIGHTS_0") != primitive.attributes.end()) {
                    const tinygltf::Accessor &uvAccessor = model.accessors[primitive.attributes.find("WEIGHTS_0")->second];
                    const tinygltf::BufferView &uvView = model.bufferViews[uvAccessor.bufferView];
                    bufferWeights = reinterpret_cast<const float *>(&(model.buffers[uvView.buffer].data[uvAccessor.byteOffset + uvView.byteOffset]));
                }

            
                vertexCount = static_cast<uint32_t>(posAccessor.count);

                for (size_t v = 0; v < posAccessor.count; v++) {
                    Vertex vert{};
                    vert.pos = glm::vec4(glm::make_vec3(&bufferPos[v * 3]), 1.0f);
                    vert.normal = glm::normalize(glm::vec3(bufferNormals ? glm::make_vec3(&bufferNormals[v * 3]) : glm::vec3(0.0f)));
                    vert.uv = bufferTexCoords ? glm::make_vec2(&bufferTexCoords[v * 2]) : glm::vec3(0.0f);
                    if (bufferColors) {
                        switch (numColorComponents) {
                            case 3: 
                                vert.color = glm::vec4(glm::make_vec3(&bufferColors[v * 3]), 1.0f);
                            case 4:
                                vert.color = glm::make_vec4(&bufferColors[v * 4]);
                        }
                    }
                    else {
                        vert.color = glm::vec4(1.0f);
                    }
                    vert.tangent = bufferTangents ? glm::vec4(glm::make_vec4(&bufferTangents[v * 4])) : glm::vec4(0.0f);
                    vert.joint0 = hasSkin ? glm::vec4(glm::make_vec4(&bufferJoints[v * 4])) : glm::vec4(0.0f);
                    vert.weight0 = hasSkin ? glm::make_vec4(&bufferWeights[v * 4]) : glm::vec4(0.0f);
                    vertexBuffer.push_back(vert);
                }
            }
            // Indices
            {
                const tinygltf::Accessor &accessor = model.accessors[primitive.indices];
                const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

                indexCount = static_cast<uint32_t>(accessor.count);

                switch (accessor.componentType) {
                case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
                    uint32_t *buf = new uint32_t[accessor.count];
                    memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(uint32_t));
                    for (size_t index = 0; index < accessor.count; index++) {
                        indexBuffer.push_back(buf[index] + vertexStart);
                    }
                    delete[] buf;
                    break;
                }
                case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
                    uint16_t *buf = new uint16_t[accessor.count];
                    memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(uint16_t));
                    for (size_t index = 0; index < accessor.count; index++) {
                        indexBuffer.push_back(buf[index] + vertexStart);
                    }
                    delete[] buf;
                    break;
                }
                case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
                    uint8_t *buf = new uint8_t[accessor.count];
                    memcpy(buf, &buffer.data[accessor.byteOffset + bufferView.byteOffset], accessor.count * sizeof(uint8_t));
                    for (size_t index = 0; index < accessor.count; index++) {
                        indexBuffer.push_back(buf[index] + vertexStart);
                    }
                    delete[] buf;
                    break;
                }
                default:
                    std::cerr << "Index component type " << accessor.componentType << " not supported!" << std::endl;
                    return;
                }
            }
        }
    }

    CHECK(true);

TEST_FUNC_END