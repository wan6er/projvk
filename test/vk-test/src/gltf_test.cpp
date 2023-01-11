#include "test_header.h"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include "tiny_gltf.h"
#include "utils/obj_data.h"
#include <string>

// bool loadImageDataFunc(tinygltf::Image* image, const int imageIndex, std::string* error, std::string* warning, int req_width, int req_height, const unsigned char* bytes, int size, void* userData)
// {
//     // KTX files will be handled by our own code
//     if (image->uri.find_last_of(".") != std::string::npos) {
//         if (image->uri.substr(image->uri.find_last_of(".") + 1) == "ktx") {
//             return true;
//         }
//     }
//     // return false;
//     return tinygltf::LoadImageData(image, imageIndex, error, warning, req_width, req_height, bytes, size, userData);
// }

typedef utils::ObjData<unsigned char> ObjectData;

TEST_FUNC_BEGIN("gltf test")

    std::string filename = "../test/vk-test/models/samplebuilding.gltf";
    std::string error, warning;
    tinygltf::TinyGLTF gltfContext;
    tinygltf::Model gltfModel;
    // gltfContext.SetImageLoader(loadImageDataFunc, nullptr);
    gltfContext.LoadASCIIFromFile(&gltfModel, &error, &warning, filename);

    ObjectData obj_data;
    for (auto& mesh : gltfModel.meshes) {
        // auto& mesh = *mesh_iter;
        for (auto& primitive : mesh.primitives) {
            for (auto& attr : primitive.attributes) {
                auto& accessor = gltfModel.accessors[attr.second];
                auto& buffer_view = gltfModel.bufferViews[accessor.bufferView];
                obj_data.append(attr.first, &gltfModel.buffers[buffer_view.buffer].data[buffer_view.byteOffset], buffer_view.byteLength);
                std::cout << attr.first << " : " << buffer_view.byteLength << "\n";
            }
        }
    }
    CHECK(true);


TEST_FUNC_END