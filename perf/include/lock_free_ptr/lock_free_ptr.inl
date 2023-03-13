
namespace utils
{
    
template<typename _Ty>
void LockFreePtr<_Ty>::operator=(LockFreePtr const& ptr)
{
    this->_obj.store(ptr._obj.load());
    this->increment(this->_obj.load(std::memory_order_relaxed));
}

template<typename _Ty>
constexpr auto LockFreePtr<_Ty>::operator*() -> Type&
{
    return *this->_obj.load(std::memory_order_relaxed)->get();
}

template<typename _Ty>
constexpr auto LockFreePtr<_Ty>::operator*() const -> Type const&
{
    return *this->_obj.load(std::memory_order_relaxed)->get();
}

template<typename _Ty>
constexpr auto LockFreePtr<_Ty>::operator->() -> Type*
{
    return this->_obj.load(std::memory_order_relaxed)->get();
}

template<typename _Ty>
constexpr auto LockFreePtr<_Ty>::operator->() const -> Type const*
{
    return this->_obj.load(std::memory_order_relaxed)->get();
}

template<typename _Ty>
constexpr auto LockFreePtr<_Ty>::operator==(void* ptr) const -> bool
{
    return this->_obj.load(std::memory_order_relaxed) == ptr;
}

template<typename _Ty>
constexpr LockFreePtr<_Ty>::operator bool() const
{
    return this->_obj.load(std::memory_order_relaxed) != nullptr;
}

    
template<typename _Ty>
constexpr LockFreeWeakPtr<_Ty>::LockFreeWeakPtr(SharedPtr const& shared_ptr)
{
    this->store(shared_ptr.load());   
}

template<typename _Ty>
constexpr auto LockFreeWeakPtr<_Ty>::operator=(SharedPtr const& ptr) -> WeakPtr&
{
    this->_obj.store(ptr.load());
    return *this;
}

template<typename _Ty>
constexpr auto LockFreeWeakPtr<_Ty>::operator=(WeakPtr const& ptr) -> WeakPtr&
{
    this->_obj.store(ptr.load());
    return *this;
}

template<typename _Ty>
constexpr auto LockFreeWeakPtr<_Ty>::operator*() -> Type&
{
    return *this->_obj.load(std::memory_order_relaxed)->get();
}

template<typename _Ty>
constexpr auto LockFreeWeakPtr<_Ty>::operator*() const -> Type const&
{
    return *this->_obj.load(std::memory_order_relaxed)->get();
}

template<typename _Ty>
constexpr auto LockFreeWeakPtr<_Ty>::operator->() -> Type*
{
    return this->_obj.load(std::memory_order_relaxed)->get();
}

template<typename _Ty>
constexpr auto LockFreeWeakPtr<_Ty>::operator->() const -> Type const*
{
    return this->_obj.load(std::memory_order_relaxed)->get();
}


template<typename _Ty>
constexpr auto LockFreeWeakPtr<_Ty>::operator==(void* ptr) const -> bool
{
    return this->_obj.load(std::memory_order_relaxed) == ptr;
}

template<typename _Ty>
constexpr LockFreeWeakPtr<_Ty>::operator bool() const
{
    return this->_obj.load(std::memory_order_relaxed) != nullptr;
}

template<typename _Ty>
constexpr auto LockFreeWeakPtr<_Ty>::get_shared() const -> SharedPtr
{
    return SharedPtr(this->load());
}


template<typename __Ty, typename...__Args>
auto make_ptr(__Args&&...args) -> LockFreePtr<__Ty>
{
    using _CountObj = CountObj<__Ty>;
    using _LockFreePtr = LockFreePtr<__Ty>;
    auto count_obj = new _CountObj;
    count_obj->construct(std::forward<__Args>(args)...);

    return _LockFreePtr(count_obj);
}

} // namespace utils
