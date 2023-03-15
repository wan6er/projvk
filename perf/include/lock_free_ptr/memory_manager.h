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
    // using _CountObj = CountObj<_Ty>;
    using _CountPtr = CountObj<_Ty>*;

    void destroy(_CountPtr ptr);

    void increment(_CountPtr obj);
    void increment_relaxed(_CountPtr obj);
    bool decrement(_CountPtr obj);

};
    
} // namespace utils

#include "memory_manager.inl"

#endif