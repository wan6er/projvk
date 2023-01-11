#pragma once

#include "cvk/vk_header.h"

#include <unordered_map>
#include <vector>

namespace utils
{

    template<class _DataType>
    class ObjData
    {
    public:
        using KeyType = std::string;
        using ValType = std::vector<_DataType>;

        // ObjData() = default;
        virtual ~ObjData();

        void append(std::string CONST_REFERENCE name, _DataType CONST_PTR data_ptr, size_t size);
        void append(std::string CONST_REFERENCE name, _DataType CONST_REFERENCE data_ptr);

        auto operator[](std::string CONST_REFERENCE name) const -> std::vector<_DataType> CONST_REFERENCE;
        auto operator[](std::string CONST_REFERENCE name) -> std::vector<_DataType>&;

    private:
        std::unordered_map<KeyType, ValType> _data;

    };

};

#include "obj_data.inl"