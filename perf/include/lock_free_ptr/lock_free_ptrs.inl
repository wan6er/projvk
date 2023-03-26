
namespace utils
{
    
template<typename _Ty, unsigned int _Size>
void LockFreePtrs<_Ty, _Size>::operator=(LockFreePtrs const& ptr)
{
    auto _o = this->load_count(MemoryOrderRelaxed);
    auto _n = ptr->load_count(MemoryOrderRelaxed);
    if (_o != ptr) {
        this->increment(_n);
        this->decrement(_o);
    }
    this->store_count(_n, MemoryOrderRelaxed);
}


template<typename _Ty, unsigned int _Size>
template<typename __Ptr>
constexpr auto LockFreePtrs<_Ty, _Size>::operator==(__Ptr&& ptr) const -> bool
{
    return this->load_count(MemoryOrderRelaxed) == reinterpret_cast<void*>(ptr.load_count(MemoryOrderRelaxed));
}

template<typename _Ty, unsigned int _Size>
template<typename __Ptr>
constexpr auto LockFreePtrs<_Ty, _Size>::operator!=(__Ptr&& ptr) const -> bool
{
    return this->load_count(MemoryOrderRelaxed) != reinterpret_cast<void*>(ptr.load_count(MemoryOrderRelaxed));
}

template<typename __Ty, unsigned int __Size, typename...__Args>
auto make_ptrs(__Args&&...args) -> LockFreePtrs<__Ty>
{
    using _CountObj = CountObjs<__Ty, __Size>;
    using _LockFreePtrs = LockFreePtrs<__Ty>;
    _CountObj count_obj;
    count_obj->construct(std::forward<__Args>(args)...);

    return _LockFreePtrs(count_obj);
}

} // namespace utils
