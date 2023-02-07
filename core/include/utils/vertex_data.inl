#pragma once

#include <sstream>

namespace utils
{

template<class _VertexType>
VectexData<_VertexType>::VectexData()
{
}

template<class _VertexType>
VectexData<_VertexType>::VectexData(std::unordered_map<std::string, VertexInfo> CONST_REFERENCE info) :
    _info(info)
{
}

template<class _VertexType>
void VectexData<_VertexType>::register_info(std::unordered_map<std::string, VertexInfo> CONST_REFERENCE info)
{
    _info = info;
}

template<class _VertexType>
void VectexData<_VertexType>::append(std::string CONST_REFERENCE name, void CONST_PTR data_ptr)
{
    CVK_ASSERT(data_ptr != nullptr);
    size_t cur_index = _append_index[name]++;
    if (cur_index >= this->objects().size()) {
        this->objects().emplace_back();
    }
    set(cur_index, name, data_ptr);
}

template<class _VertexType>
void VectexData<_VertexType>::append(_VertexType CONST_REFERENCE vertex)
{
    this->objects().emplace_back(vertex);
}

template<class _VertexType>
void VectexData<_VertexType>::set(size_t index, std::string CONST_REFERENCE name, void CONST_PTR data_ptr)
{
    CVK_ASSERT(data_ptr != nullptr);
    CVK_ASSERT(index < this->objects().size());
    VertexInfo CONST_REFERENCE cur_info = _info[name];
    memcpy(cur_info.offset + reinterpret_cast<char*>(&this->objects()[index]), data_ptr, cur_info.size);
}

template<class _VertexType>
auto VectexData<_VertexType>::get_info(std::string CONST_REFERENCE name) const -> VertexInfo CONST_REFERENCE
{
    auto iter = _info.find(name);
    if (iter == _info.end()) {
        throw std::exception();
    }
    return iter->second;
}

template<class _VertexType>
size_t VectexData<_VertexType>::get_size() const
{
    return this->objects().size();
}

template<class _VertexType>
size_t VectexData<_VertexType>::get_data_size() const
{
    return this->objects().size() * sizeof(_VertexType);
}

template<class _VertexType>
auto VectexData<_VertexType>::get_data() const -> _VertexType CONST_PTR
{
    return this->objects().data();
}

} // namespace utils