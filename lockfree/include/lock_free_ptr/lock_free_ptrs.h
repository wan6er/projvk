#ifndef __LOCK_FREE_PTRS_T__
#define __LOCK_FREE_PTRS_T__

#include "lock_free_ptr_base.h"
#include "lock_free_weak_ptr_base.h"
#include "shared_ptrs.h"
#include "list_memory_manager.h"

namespace utils
{

template<typename _Ty, unsigned int _Size>
struct LockFreePtrs : 
    public BaseLockFreePtr<_Ty, SharedPtrs<_Ty, _Size>, CountObjs<_Ty, _Size>, ListMemoryManager<_Ty, _Size> >
{
    using Type = _Ty;
    using Ptr = SharedPtr<_Ty>;
    using _CountObj = CountObjs<_Ty, _Size>;
    using _CountPtr = CountPtr<_Ty>;
    using _Base = BaseLockFreePtr<_Ty, SharedPtrs<_Ty, _Size>, CountObjs<_Ty, _Size>, ListMemoryManager<_Ty, _Size> >;
    using _Base::_Base;
    using _MemoryManager = MemoryManager<_Ty, _CountObj>;
  
    void operator=(LockFreePtrs const& ptr);

    void swap(unsigned int index, _CountPtr ptr) {
        _MemoryManager::increment(ptr);
        _MemoryManager::decrement(this->_obj[index]);
        this->_obj[index] = ptr;
    }

    auto operator[](unsigned int index) const { return this->_obj[index]; }
    auto operator[](unsigned int index) -> _CountPtr& { return this->_obj[index]; }

    template<typename __Ptr>
    constexpr auto operator==(__Ptr&& ptr) const -> bool;
    template<typename __Ptr>
    constexpr auto operator!=(__Ptr&& ptr) const -> bool;
    // constexpr operator bool() const;
    // auto get_snapshot() const -> AtomicWeakPtr { return AtomicWeakPtr(new _CountObj(this->load_count())); }

};

template<typename _Ty, unsigned int _Size, typename..._Args>
auto make_ptrs(_Args&&...args)
{
    using _CountObjs = CountObjs<_Ty, _Size>;
    using _Ptrs = LockFreePtrs<_Ty, _Size>;
    _CountObjs objs;
    for (unsigned int i = 0; i < objs.size; ++i) {
        objs[i] = make_count_obj<_Ty>();
        objs[i]->construct(std::forward<_Args>(args)...);
    }
    return _Ptrs(objs);
}

} // namespace utils


#endif