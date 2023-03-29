#ifndef __LIST_MEMORY_MANAGER_H__
#define __LIST_MEMORY_MANAGER_H__

#include "clf_basic.h"
#include "memory_manager.h"
#include "count_obj.h"
#include "compatible_atomic.h"

namespace utils
{

template<typename _Ty, unsigned int _Size>
struct ListMemoryManager : public MemoryManager<_Ty, CountPtr<_Ty> >
{
    using _MMBase = MemoryManager<_Ty, CountPtr<_Ty> >;
    using _CountObj = CountObjs<_Ty, _Size>&;

    void destroy(_CountObj ptr);

    void increment(_CountObj obj);
    void increment_relaxed(_CountObj obj);
    void decrement(_CountObj obj);

};
    
} // namespace utils

#include "list_memory_manager.inl"

#endif