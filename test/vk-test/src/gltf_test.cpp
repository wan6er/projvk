#include "test_header.h"
// #define TINYGLTF_IMPLEMENTATION
// #define STB_IMAGE_IMPLEMENTATION
// #define TINYGLTF_NO_STB_IMAGE_WRITE
#include "tiny_gltf.h"
#include "utils/tuple_map.h"
#include "utils/vertex_data.h"
#include "utils/def_utils.h"
#include <tuple>
#include <glm/glm.hpp>
#include <string>


TEST_FUNC_BEGIN("gltf test")

    std::string filename = "../test/vk-test/models/samplebuilding.gltf";
    std::string error, warning;
    tinygltf::TinyGLTF gltfContext;
    tinygltf::Model gltfModel;
    // gltfContext.SetImageLoader(loadImageDataFunc, nullptr);
    gltfContext.LoadASCIIFromFile(&gltfModel, &error, &warning, filename);

    struct Vertex
    {
        glm::vec3 POSITION;
        glm::vec3 NORMAL;
        glm::vec2 TEXCOORD_0;
    };
    utils::VectexData<Vertex> vertexes(VERTEX_DATA_REG_MEMBERS(Vertex, POSITION, NORMAL, TEXCOORD_0));
    std::vector<uint32_t> indices;

    for (auto& mesh : gltfModel.meshes) {
        for (auto& primitive : mesh.primitives) {
            auto& index_accessor = gltfModel.accessors[primitive.indices];
            auto& index_buffer_view = gltfModel.bufferViews[index_accessor.bufferView];
            for (uint32_t i = 0; i < index_accessor.count; ++i) {
                // indices.emplace_back(gltfModel.buffers);
            }

            for (auto& attr : primitive.attributes) {
                auto& accessor = gltfModel.accessors[attr.second];
                auto& buffer_view = gltfModel.bufferViews[accessor.bufferView];

                for (uint32_t i = 0; i < accessor.count; ++i) {
                    vertexes.append(attr.first, &gltfModel.buffers[buffer_view.buffer].data[i * vertexes.get_info(attr.first).size + buffer_view.byteOffset + accessor.byteOffset]);
                }
            }
        }
    }

TEST_FUNC_END