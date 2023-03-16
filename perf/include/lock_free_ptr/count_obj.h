#ifndef __COUNT_OBJ_H__
#define __COUNT_OBJ_H__

#include "compatible_atomic.h"

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

    auto get() -> _Ty*;
    auto get() const -> _Ty const*;

    auto add_refs(MemoryOrder order = MemoryOrderSeqCst) -> RefCount;
    auto sub_refs(MemoryOrder order = MemoryOrderSeqCst) -> RefCount;

    template<typename...__Args>
    void construct(__Args&&...args);
    void construct();
    void release();

};

} // namespace utils

#include "count_obj.inl"

#endif