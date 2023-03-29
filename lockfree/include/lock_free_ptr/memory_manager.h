#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

#include "clf_basic.h"
#include "memory_manager_base.h"
#include "count_obj.h"
#include "compatible_atomic.h"

namespace utils
{

template<typename _Ty, typename _CountObj>
struct MemoryManager : public BaseMemoryManager
{
    // using _CountObj = CountObj<_Ty>;

    void destroy(_CountObj ptr);

    void increment(_CountObj obj);
    void increment_relaxed(_CountObj obj);
    bool decrement(_CountObj obj);

};
    
} // namespace utils

#include "memory_manager.inl"

#endif