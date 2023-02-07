#pragma once

#include "tiny_gltf.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "cvk/vk_header.h"
#include "utils/vertex_data.h"

namespace cgltf
{
    struct CVK_API NodeTransform
    {
        glm::vec3 translation;
        glm::vec3 scale;
        glm::quat rotation;

        NodeTransform();

        glm::mat4 to_matrix() const;
    };

    template<class _VertexType, class _IndexType>
    class Node
    {
    public:
        // Node() = default;
        virtual ~Node() = default;
        
        virtual void setup_info(std::unordered_map<std::string, utils::VertexInfo> CONST_REFERENCE info);
        virtual void load(tinygltf::Model CONST_REFERENCE model, tinygltf::Node CONST_REFERENCE node);
        virtual void load_material(tinygltf::Model CONST_REFERENCE model, tinygltf::Node CONST_REFERENCE node, std::string CONST_REFERENCE color);

        auto get_vertexes() const -> utils::VectexData<_VertexType> CONST_REFERENCE;
        auto get_indices() const -> std::vector<_IndexType> CONST_REFERENCE;
        auto get_transform() const -> NodeTransform CONST_REFERENCE;

    protected:
        void load_vertex(tinygltf::Model CONST_REFERENCE model, tinygltf::Node CONST_REFERENCE node);
        void load_index(tinygltf::Model CONST_REFERENCE model, tinygltf::Node CONST_REFERENCE node);
        void load_matrix(tinygltf::Model CONST_REFERENCE model, tinygltf::Node CONST_REFERENCE node);

    private:
        utils::VectexData<_VertexType> _vertex;
        std::vector<_IndexType> _index;

        NodeTransform _transform;
    };

} // namespace gltftools

#include "gltf_node.inl"