#pragma once

#include <algorithm>
#include <type_traits>

#include "clog.h"

namespace utils
{


template <class Type>
void vector_filter(const std::vector<Type> &src_vec, const std::vector<Type> &trg_vec, std::vector<Type> &out_vec)
{
    for (auto item : trg_vec) 
    {
        if (std::find(src_vec.begin(), src_vec.end(), item) != src_vec.end()) 
        {
            out_vec.push_back(item);
        }
        else 
        {
#if defined(__cplusplus) and __cplusplus >= 201703L
            if constexpr (std::is_same_v<Type, std::string>) {
                CLogE("Filter %s failed.", item.c_str());
            }
#endif
        }
    }
}

template <class __Type>
void vector_fill_info(std::vector<__Type> CONST_REFERENCE array, uint32_t& count, __Type CONST_PTR& ptr)
{
    count = static_cast<uint32_t>(array.size());
    ptr = array.size() == 0 ? nullptr : array.data();
}

template<class __SrcType, class __DstType>
void vector_transition(std::vector<__SrcType> CONST_REFERENCE src, std::vector<__DstType>& dst, std::function<void(__SrcType CONST_REFERENCE, __DstType&)> transitional)
{
    for (auto i : src) {
        __DstType _t;
        transitional(i, _t);
        dst.emplace_back(_t);
    }
}

template <class __Type>
void vector_iter(std::vector<__Type> CONST_REFERENCE array, std::function<void(__Type CONST_REFERENCE)> iter)
{
    for (auto i : array) {
        iter(i);
    }
}

};
