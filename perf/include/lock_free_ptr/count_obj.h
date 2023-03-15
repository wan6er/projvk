#ifndef __COUNT_OBJ_H__
#define __COUNT_OBJ_H__

#include "compatible_atomic.h"

namespace utils
{

template<typename _Ty>
struct CountObj
{
    alignas(alignof(_Ty)) unsigned int storage[sizeof(_Ty)] = {};
    Atomic<unsigned int> ref_cnt;

    CountObj() : ref_cnt(0) {}
    CountObj(CountObj const&) = delete;

    auto get() -> _Ty*;
    auto get() const -> _Ty const*;

    void add_refs(std::memory_order order = std::memory_order_seq_cst);
    void sub_refs(std::memory_order order = std::memory_order_seq_cst);

    template<typename...__Args>
    void construct(__Args&&...args);
    void construct();
    void release();

};

} // namespace utils

#include "count_obj.inl"

#endif