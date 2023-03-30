#ifndef __COUNT_OBJ_H__
#define __COUNT_OBJ_H__

#include "compatible_atomic.h"

#include <string.h>

namespace utils
{

template<typename _Ty>
struct CountObj
{
    using RefCount = unsigned int;

    alignas(alignof(_Ty)) unsigned int storage[sizeof(_Ty)] = {};
    Atomic<RefCount> ref_cnt;

    CountObj() : ref_cnt(0) {}
    CountObj(CountObj const&) = delete;
    // CountObj(CountObj const& c) : ref_cnt(c.ref_cnt.load(MemoryOrderRelaxed)) { memcpy(storage, c.storage, sizeof(storage)); }

    auto operator->() -> _Ty* { return get(); }
    auto operator->() const -> _Ty const* { return get(); }

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
using CountPtr = CountObj<_Ty>*;

template<typename _Ty>
auto make_count_obj() -> CountPtr<_Ty>
{
    return new CountObj<_Ty>;
}

template<typename _Ty>
struct CountObjR
{
    using _CountPtr = CountObj<_Ty>*;
    _CountPtr ptr = nullptr;
    uintptr_t r_cnt = 0;

    CountObjR() {}
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

template<typename _Ty, unsigned int _Size>
struct CountObjs
{
    constexpr static unsigned int size = _Size;
    using _CountPtr = CountObj<_Ty>*;
    using _CountPtrs = _CountPtr[size];
    _CountPtrs ptr = {};
    uintptr_t r_cnt = 0;

    CountObjs() {}
    CountObjs(_CountPtrs const ptrs, uint32_t read_count) : r_cnt(read_count) { memcpy(ptr, ptrs, sizeof(_CountPtrs)); }
    CountObjs(_CountPtrs const ptrs) : CountObjs(ptrs, 0) {}
    // CountObjs(uint32_t read_count) : CountObjs() { r_cnt = read_count; }
    // CountObjs(CountObjs const& other) : CountObjs(other.ptr, other.r_cnt) {}
    CountObjs(CountObjs const&) = default;

    CountObjs add_cnt() { return CountObjs(ptr, r_cnt + 1); }
    CountObjs sub_cnt() { return CountObjs(ptr, r_cnt - 1); }

    auto get_count() -> uintptr_t { return r_cnt; }

    void destory(unsigned int index) { 
        auto _tmp = reinterpret_cast<_CountPtr>(ptr[index]);
        ptr[index] = 0;
        delete _tmp;
    } 

    auto operator[](unsigned int index) const -> _CountPtr { return ptr[index]; }
    auto operator[](unsigned int index) -> _CountPtr& { return ptr[index]; }

    // bool operator==(void* other) const { return !memcmp(ptr, other); }
    bool operator==(CountObjs const& other) const { return !memcmp(ptr, other.ptr) && r_cnt == other.r_cnt; }
    // bool operator!=(void* other) const { return !(*this == other); }
    bool operator!=(CountObjs const& other) const { return !(*this == other); }
};

} // namespace utils

#include "count_obj.inl"

#endif