#pragma once

#include "cvk/vk_header.h"

#include <vector>
#include <string>
#include <functional>

namespace utils
{
    template <class Type>
    void vector_filter(const std::vector<Type> &src_vec, const std::vector<Type> &trg_vec, std::vector<Type> &out_vec);

    template <class __Type>
    void vector_fill_info(std::vector<__Type> CONST_REFERENCE array, uint32_t& count, __Type CONST_PTR& ptr);
    
    template <class __Type>
    void vector_iter(std::vector<__Type> CONST_REFERENCE array, std::function<void(__Type CONST_REFERENCE)> iter);

    template<class __SrcType, class __DstType>
    void vector_transition(std::vector<__SrcType> CONST_REFERENCE src, std::vector<__DstType>& dst, std::function<void(__SrcType CONST_REFERENCE, __DstType&)> transitional);
    // void vector_transition(std::vector<__SrcType> CONST_REFERENCE src, std::vector<__DstType>& dst);

    CVK_API void string_to_charptr(const std::vector<std::string>& src, std::vector<const char*>& dst);
};

#include "utils/vector_util.inl"