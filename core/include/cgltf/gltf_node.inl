#pragma once

// #include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace cgltf
{

template<class _VertexType, class _IndexType>
void Node<_VertexType, _IndexType>::setup_info(std::unordered_map<std::string, utils::VertexInfo> CONST_REFERENCE info)
{
    _vertex.register_info(info);
}

template<class _VertexType, class _IndexType>
void Node<_VertexType, _IndexType>::load(tinygltf::Model CONST_REFERENCE model, tinygltf::Node CONST_REFERENCE node)
{
    // load_vertex_index(model, node);
    load_vertex(model, node);
    load_index(model, node);
    load_matrix(model, node);
}

template<class _VertexType, class _IndexType>
void Node<_VertexType, _IndexType>::load_vertex_index(tinygltf::Model CONST_REFERENCE model, tinygltf::Node CONST_REFERENCE node)
{
    auto CONST_REFERENCE mesh = model.meshes[node.mesh];
    for (auto CONST_REFERENCE primitive : mesh.primitives) {
        uint16_t vertex_start = _vertex.get_size();

        auto& index_accessor = model.accessors[primitive.indices];
        auto& index_buffer_view = model.bufferViews[index_accessor.bufferView];
        auto index_buffer = reinterpret_cast<_IndexType CONST_PTR>(&model.buffers[index_buffer_view.buffer].data[index_accessor.byteOffset + index_buffer_view.byteOffset]);
        _index.reserve(_index.size() + index_accessor.count);
        for (uint32_t i = 0; i < index_accessor.count; ++i) {
            _index.emplace_back(index_buffer[i] + vertex_start);
        }

        for (auto CONST_REFERENCE attr : primitive.attributes) {
            auto CONST_REFERENCE accessor = model.accessors[attr.second];
            auto CONST_REFERENCE buffer_view = model.bufferViews[accessor.bufferView];
            if (vertex_start + accessor.count > _vertex.get_size()) {
                _vertex.resize(_vertex.get_size() + accessor.count);
            }

            for (uint32_t i = 0; i < accessor.count; ++i) {
                _vertex.set(i + vertex_start, attr.first, &model.buffers[buffer_view.buffer].data[i * _vertex.get_info(attr.first).size + buffer_view.byteOffset + accessor.byteOffset]);
            }
        }
    }

}


template<class _VertexType, class _IndexType>
void Node<_VertexType, _IndexType>::load_vertex(tinygltf::Model CONST_REFERENCE model, tinygltf::Node CONST_REFERENCE node)
{
    auto CONST_REFERENCE mesh = model.meshes[node.mesh];
    for (auto CONST_REFERENCE primitive : mesh.primitives) {
        const uint16_t vertex_start = _vertex.get_size();

        for (auto CONST_REFERENCE attr : primitive.attributes) {
            auto CONST_REFERENCE accessor = model.accessors[attr.second];
            auto CONST_REFERENCE buffer_view = model.bufferViews[accessor.bufferView];
            if (vertex_start + accessor.count > _vertex.get_size()) {
                _vertex.resize(_vertex.get_size() + accessor.count);
            }

            for (uint32_t i = 0; i < accessor.count; ++i) {
                _vertex.set(i + vertex_start, attr.first, &model.buffers[buffer_view.buffer].data[i * _vertex.get_info(attr.first).size + buffer_view.byteOffset + accessor.byteOffset]);
            }
        }
    }
}

template<class _VertexType, class _IndexType>
void Node<_VertexType, _IndexType>::load_index(tinygltf::Model CONST_REFERENCE model, tinygltf::Node CONST_REFERENCE node)
{
    uint16_t vertex_start = 0;
    auto CONST_REFERENCE mesh = model.meshes[node.mesh];
    for (auto CONST_REFERENCE primitive : mesh.primitives) {
        auto& index_accessor = model.accessors[primitive.indices];
        auto& index_buffer_view = model.bufferViews[index_accessor.bufferView];
        auto index_buffer = reinterpret_cast<_IndexType CONST_PTR>(&model.buffers[index_buffer_view.buffer].data[index_accessor.byteOffset + index_buffer_view.byteOffset]);
        
        _index.reserve(_index.size() + index_accessor.count);
        for (uint32_t i = 0; i < index_accessor.count; ++i) {
            _index.emplace_back(index_buffer[i] + vertex_start);
        }

        if (primitive.attributes.size() > 0) {
            auto CONST_REFERENCE vertex_accessor = model.accessors[primitive.attributes.begin()->second];
            vertex_start += vertex_accessor.count;
        }
    }
}

template<class _VertexType, class _IndexType>
void Node<_VertexType, _IndexType>::load_matrix(tinygltf::Model CONST_REFERENCE model, tinygltf::Node CONST_REFERENCE node)
{
    if (node.translation.size() == 3) {
        _transform.translation = glm::make_vec3(node.translation.data());
    }
    if (node.rotation.size() == 4) {
        _transform.rotation = glm::make_quat(node.rotation.data());
    }
    if (node.scale.size() == 3) {
        _transform.scale = glm::make_vec3(node.scale.data());
    }
    // if (node.matrix.size() == 16) {
    //     newNode->matrix = glm::make_mat4x4(node.matrix.data());
    //     if (globalscale != 1.0f) {
    //         //newNode->matrix = glm::scale(newNode->matrix, glm::vec3(globalscale));
    //     }
    // };
}

template<class _VertexType, class _IndexType>
auto Node<_VertexType, _IndexType>::get_vertexes() const -> utils::VectexData<_VertexType> CONST_REFERENCE
{
    return _vertex;
}

template<class _VertexType, class _IndexType>
auto Node<_VertexType, _IndexType>::get_indices() const -> std::vector<_IndexType> CONST_REFERENCE
{
    return _index;
}

template<class _VertexType, class _IndexType>
auto Node<_VertexType, _IndexType>::get_transform() const -> NodeTransform CONST_REFERENCE
{
    return _transform;
}
    
} // namespace gltfutils
