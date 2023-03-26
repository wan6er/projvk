#ifndef __SHARED_PTRS_H__
#define __SHARED_PTRS_H__

#include "shared_ptr.h"
#include "list_memory_manager.h"

namespace utils
{
    
template<typename _Ty, unsigned int _Size>
class SharedPtrs :
    public BaseSharedPtrImpl<_Ty, CountObjs<_Ty, _Size>, SharedPtrs<_Ty, _Size> >, 
    public ListMemoryManager<_Ty, _Size>
{
public:
    constexpr static unsigned int size = _Size;

    using Type = _Ty;
    using Ptr = SharedPtr<_Ty>;
    using Ptrs = Ptr[size];

protected:
    using _CountPtr = CountPtr<_Ty>;
    using _CountObjs = CountObjs<_Ty, size>;
    using _Base = BaseSharedPtrImpl<_Ty, _CountObjs, SharedPtrs<_Ty, _Size>>;
    // using _BaseMM = ListMemoryManager<_Ty, size>;
    using _MemoryManager = MemoryManager<_Ty, CountPtr<_Ty> >;

public:
    SharedPtrs();
    SharedPtrs(_CountObjs& objs);
    ~SharedPtrs();
        
    void swap(_CountObjs& ptr);
    void swap(unsigned int index, _CountPtr ptr);
    
    void operator=(SharedPtrs const& ptr);

    auto operator[](unsigned int index) const -> _CountPtr { return this->get_count_ref()[index]; }
    auto operator[](unsigned int index) -> _CountPtr& { return this->get_count_ref()[index]; }

};

template<typename _Ty, unsigned int _Size, typename..._Args>
auto make_shared_ptrs(_Args&&...args);

}

#include "shared_ptrs.inl"

#endif