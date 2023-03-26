#include "shared_ptrs.h"

namespace utils
{
    

template<typename _Ty, unsigned int _Size>
SharedPtrs<_Ty, _Size>::SharedPtrs() :
    _Base()
{

}

template<typename _Ty, unsigned int _Size>
SharedPtrs<_Ty, _Size>::SharedPtrs(_CountObjs& objs) :
    _Base(objs)
{
    this->increment(this->get_count_ref());
}

template<typename _Ty, unsigned int _Size>
SharedPtrs<_Ty, _Size>::~SharedPtrs()
{
    this->decrement(this->get_count_ref());
}

template<typename _Ty, unsigned int _Size>
void SharedPtrs<_Ty, _Size>::swap(_CountObjs& ptr)
{
    auto _o = this->get_count();
    this->increment(ptr);
    this->decrement(_o);
    this->get_count_ref() = ptr;
}

template <typename _Ty, unsigned int _Size>
inline void SharedPtrs<_Ty, _Size>::swap(unsigned int index, _CountPtr ptr)
{
    _MemoryManager::increment(ptr);
    _MemoryManager::decrement((*this)[index]);
    this->_obj[index] = ptr;
}

template <typename _Ty, unsigned int _Size>
void SharedPtrs<_Ty, _Size>::operator=(SharedPtrs const &ptr)
{
    auto _o = this->get_count();
    auto _n = ptr.get_count();
    this->increment(_n);
    this->decrement(_o);
    this->get_count_ref() = _n;

}

template<typename _Ty, unsigned int _Size, typename..._Args>
auto make_shared_ptrs(_Args&&...args)
{
    using _CountObjs = CountObjs<_Ty, _Size>;
    using _Ptrs = SharedPtrs<_Ty, _Size>;
    _CountObjs objs;
    for (unsigned int i = 0; i < objs.size; ++i) {
        objs[i] = make_count_obj<_Ty>();
        objs[i]->construct(std::forward<_Args>(args)...);
    }
    return _Ptrs(objs);
}

} // namespace utils
