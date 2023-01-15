#pragma once

#include "cvk/vk_header.h"
#include "utils/def_utils.h"

#include <unordered_map>

#define _VERTEX_DATA_REG_MEMBER_(obj, name) {#name , { offsetof(obj, name), sizeof(obj::name) }},
#define VERTEX_DATA_REG_MEMBERS(obj, ...) { CVK_DEF_FOREACH(, obj, _VERTEX_DATA_REG_MEMBER_, __VA_ARGS__) }

namespace utils
{
    
    template<class _VertexType>
    class VectexData
    {
    public:
        struct VertexInfo
        {
            size_t offset;
            size_t size;
        };
        // using KeyType = std::string;

        VectexData(std::unordered_map<std::string, VertexInfo> CONST_REFERENCE info);
        virtual ~VectexData() = default;

        void resize(size_t size);
        void reserve(size_t size);
        void append(std::string CONST_REFERENCE name, void* data_ptr);
        void append(_VertexType CONST_REFERENCE vertex);
        void set(size_t index, std::string CONST_REFERENCE name, void* data_ptr);
        size_t get_size() const;
        size_t get_num_vertex() const;
        auto get_info(std::string CONST_REFERENCE name) const -> VertexInfo CONST_REFERENCE;
        auto get_data() const -> _VertexType CONST_PTR;

        auto operator[](size_t index) -> _VertexType&;
        auto operator[](size_t index) const -> _VertexType CONST_REFERENCE;

    private:
        std::unordered_map<std::string, VertexInfo> _info;
        std::unordered_map<std::string, size_t> _append_index;
        std::vector<_VertexType> _data;

    };

} // namespace utils

#include "vertex_data.inl"
