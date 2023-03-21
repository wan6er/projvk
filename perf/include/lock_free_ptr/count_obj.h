#ifndef __COUNT_OBJ_H__
#define __COUNT_OBJ_H__

#include "compatible_atomic.h"

namespace utils
{

template<typename _Ty>
struct CountObj
{
    using RefCount = unsigned int;

    unsigned int storage[sizeof(_Ty)] = {};
    Atomic<RefCount> ref_cnt;

    CountObj() : ref_cnt(0) {}
    CountObj(CountObj const&) = delete;
    // CountObj(CountObj const& c) : ref_cnt(c.ref_cnt.load(MemoryOrderRelaxed)) { memcpy(storage, c.storage, sizeof(storage)); }

    auto get() -> _Ty*;
    auto get() const -> _Ty const*;

    auto add_refs(MemoryOrder order = MemoryOrderSeqCst) -> RefCount;
    auto sub_refs(MemoryOrder order = MemoryOrderSeqCst) -> RefCount;

    template<typename...__Args>
    void construct(__Args&&...args);
    void construct();
    void release();

};

template<typename _Ty>
struct CountObjR
{
    using _CountPtr = CountObj<_Ty>*;
    // union {
        _CountPtr ptr;
        //uintptr_t ptr;
    // };
    uintptr_t r_cnt = 0;

    CountObjR() : ptr(new CountObj<_Ty>) {}
    CountObjR(void* ptr) : ptr((_CountPtr) ptr) {}
    // constexpr CountObjR(_CountPtr ptr) : ptr((uintptr_t)ptr) {}
    CountObjR(_CountPtr ptr, uint32_t read_count) : ptr(ptr), r_cnt(read_count) {}
    CountObjR(CountObjR const&) = default;

    CountObjR add_cnt() { return CountObjR(ptr, r_cnt + 1); }
    CountObjR sub_cnt() { return CountObjR(ptr, r_cnt - 1); }

    auto get() -> _CountPtr { return (ptr); }
    auto get_count() -> uintptr_t { return r_cnt; }

    void destory() { auto tmp = get(); ptr = 0; delete tmp; } 

    auto operator->() -> _CountPtr { return get(); }
    bool operator==(void* other) const { return ptr == (_CountPtr) other; }
    bool operator==(CountObjR const& other) const { return ptr == other.ptr; }
    bool operator!=(void* other) const { return ptr != (_CountPtr) other; }
    bool operator!=(CountObjR const& other) const { return ptr != other.ptr; }
};

} // namespace utils

#include "count_obj.inl"

#endif