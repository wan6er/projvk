#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

#include "perf_basic.h"
#include "memory_manager_base.h"
#include "count_obj.h"
#include "compatible_atomic.h"

namespace utils
{

template<typename _Ty>
struct MemoryManager : public BaseMemoryManager
{
    using _CountObj = CountObj<_Ty>;
    using _CountObjPtr = _CountObj*;

    void destroy(_CountObjPtr ptr);

    void increment(_CountObjPtr obj);
    void increment_relaxed(_CountObjPtr obj);
    bool decrement(_CountObjPtr obj);

};
    
} // namespace utils

#include "memory_manager.inl"

#endif