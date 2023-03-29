
namespace utils
{
    
template<typename _Ty>
void LockFreePtr<_Ty>::operator=(LockFreePtr const& ptr)
{
    auto _o = this->load_count(MemoryOrderRelaxed);
    auto _n = ptr->load_count(MemoryOrderRelaxed);
    if (_o != ptr) {
        this->increment(_n);
        this->decrement(_o);
    }
    this->store_count(_n, MemoryOrderRelaxed);
}

template<typename _Ty>
constexpr auto LockFreePtr<_Ty>::operator*() -> Type&
{
    return *this->_obj.load(MemoryOrderRelaxed)->get();
}

template<typename _Ty>
constexpr auto LockFreePtr<_Ty>::operator*() const -> Type const&
{
    return *this->_obj.load(MemoryOrderRelaxed)->get();
}

template<typename _Ty>
constexpr auto LockFreePtr<_Ty>::operator->() -> Type*
{
    return this->_obj.load(MemoryOrderRelaxed)->get();
}

template<typename _Ty>
constexpr auto LockFreePtr<_Ty>::operator->() const -> Type const*
{
    return this->_obj.load(MemoryOrderRelaxed)->get();
}


template<typename _Ty>
template<typename __Ptr>
constexpr auto LockFreePtr<_Ty>::operator==(__Ptr&& ptr) const -> bool
{
    return this->load_count(MemoryOrderRelaxed) == reinterpret_cast<void*>(ptr.load_count(MemoryOrderRelaxed));
}

template<typename _Ty>
template<typename __Ptr>
constexpr auto LockFreePtr<_Ty>::operator!=(__Ptr&& ptr) const -> bool
{
    return this->load_count(MemoryOrderRelaxed) != reinterpret_cast<void*>(ptr.load_count(MemoryOrderRelaxed));
}

// template<typename _Ty>
// constexpr LockFreePtr<_Ty>::operator bool() const
// {
//     return this->_obj.load(MemoryOrderRelaxed) != nullptr;
// }
    
template<typename _Ty>
constexpr LockFreeWeakPtr<_Ty>::LockFreeWeakPtr(AtomicSharedPtr const& shared_ptr)
{
    // this->store_count(shared_ptr.load_count(MemoryOrderRelaxed), MemoryOrderAcquire);   
    (*this) = shared_ptr;
}

template<typename _Ty>
constexpr auto LockFreeWeakPtr<_Ty>::operator=(AtomicSharedPtr const& ptr) -> AtomicWeakPtr&
{
    this->store_count(ptr.load_count(MemoryOrderRelaxed), MemoryOrderAcquire);
    return *this;
}

template<typename _Ty>
constexpr auto LockFreeWeakPtr<_Ty>::operator=(AtomicWeakPtr const& ptr) -> AtomicWeakPtr&
{
    this->store_count(ptr.load_count(MemoryOrderRelaxed), MemoryOrderAcquire);
    return *this;
}

template<typename _Ty>
constexpr auto LockFreeWeakPtr<_Ty>::operator*() -> Type&
{
    return *this->_obj.load(MemoryOrderRelaxed)->get();
}

template<typename _Ty>
constexpr auto LockFreeWeakPtr<_Ty>::operator*() const -> Type const&
{
    return *this->_obj.load(MemoryOrderRelaxed)->get();
}

template<typename _Ty>
constexpr auto LockFreeWeakPtr<_Ty>::operator->() -> Type*
{
    return this->_obj.load(MemoryOrderRelaxed)->get();
}

template<typename _Ty>
constexpr auto LockFreeWeakPtr<_Ty>::operator->() const -> Type const*
{
    return this->_obj.load(MemoryOrderRelaxed)->get();
}


template<typename _Ty>
template<typename __Ptr>
constexpr auto LockFreeWeakPtr<_Ty>::operator==(__Ptr&& ptr) const -> bool
{
    return this->load_count(MemoryOrderRelaxed) == reinterpret_cast<void*>(ptr.load_count(MemoryOrderRelaxed));
}

template<typename _Ty>
template<typename __Ptr>
constexpr auto LockFreeWeakPtr<_Ty>::operator!=(__Ptr&& ptr) const -> bool
{
    return this->load_count(MemoryOrderRelaxed) != reinterpret_cast<void*>(ptr.load_count(MemoryOrderRelaxed));
}

// template<typename _Ty>
// constexpr LockFreeWeakPtr<_Ty>::operator bool() const
// {
//     return this->_obj.load(MemoryOrderRelaxed) != nullptr;
// }

template<typename _Ty>
constexpr auto LockFreeWeakPtr<_Ty>::get_shared() const -> AtomicSharedPtr
{
    return AtomicSharedPtr(this->load());
}


template<typename __Ty, typename...__Args>
auto make_ptr(__Args&&...args) -> LockFreePtr<__Ty>
{
    using _CountObj = CountObjR<__Ty>;
    using _LockFreePtr = LockFreePtr<__Ty>;
    _CountObj count_obj(make_count_obj<__Ty>());
    count_obj->construct(std::forward<__Args>(args)...);

    return _LockFreePtr(count_obj);
}

} // namespace utils
