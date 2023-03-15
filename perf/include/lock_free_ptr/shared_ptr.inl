
namespace utils
{

    
template<typename _Ty>
constexpr SharedPtr<_Ty>::SharedPtr() :
    _Base()
{
}

template<typename _Ty>
constexpr SharedPtr<_Ty>::SharedPtr(_CountPtr count) :
    _Base(count)
{
        // this->_obj->add_refs(std::memory_order_relaxed);
    this->increment(this->_obj);
}

template<typename _Ty>
constexpr SharedPtr<_Ty>::SharedPtr(SharedPtr const& ptr) :
    _Base(ptr)
{
    this->increment(this->_obj);
}

template<typename _Ty>
SharedPtr<_Ty>::~SharedPtr()
{
    if (this->decrement(this->_obj)) {
        this->destroy(this->_obj);
        this->_obj = nullptr;
    }
}
    
template<typename _Ty>
void SharedPtr<_Ty>::operator=(SharedPtr const& ptr)
{
    auto old = this->_obj;
    this->increment(ptr._obj);
    this->_obj = ptr._obj;
    if (old != this->_obj) {
        this->decrement(old);
    }
}

template<typename _Ty>
constexpr auto SharedPtr<_Ty>::operator*() -> Type&
{
    return *this->_obj->get();
}

template<typename _Ty>
constexpr auto SharedPtr<_Ty>::operator*() const -> Type const&
{
    return *this->_obj->get();
}

template<typename _Ty>
constexpr auto SharedPtr<_Ty>::operator->() -> Type*
{
    return this->_obj->get();
}

template<typename _Ty>
constexpr auto SharedPtr<_Ty>::operator->() const -> Type const*
{
    return this->_obj->get();
}

template<typename _Ty>
constexpr auto SharedPtr<_Ty>::operator==(void* ptr) const -> bool
{
    return this->_obj == ptr;
}

template<typename _Ty>
constexpr SharedPtr<_Ty>::operator bool() const
{
    return this->_obj != nullptr;
}

    
template<typename _Ty>
constexpr auto WeakPtr<_Ty>::operator=(_SharedPtr const& ptr) -> WeakPtr&
{
    this->_obj = ptr._obj;
    return *this;
}

template<typename _Ty>
constexpr auto WeakPtr<_Ty>::operator=(WeakPtr const& ptr) -> WeakPtr&
{
    // this->_obj.store(ptr.load());
    this->_obj = ptr._obj;
    return *this;
}

template<typename _Ty>
constexpr auto WeakPtr<_Ty>::operator*() -> Type&
{
    return *this->_obj->get();
}

template<typename _Ty>
constexpr auto WeakPtr<_Ty>::operator*() const -> Type const&
{
    return *this->_obj->get();
}

template<typename _Ty>
constexpr auto WeakPtr<_Ty>::operator->() -> Type*
{
    return this->_obj->get();
}

template<typename _Ty>
constexpr auto WeakPtr<_Ty>::operator->() const -> Type const*
{
    return this->_obj->get();
}


template<typename _Ty>
constexpr auto WeakPtr<_Ty>::operator==(void* ptr) const -> bool
{
    return this->_obj == ptr;
}

template<typename _Ty>
constexpr WeakPtr<_Ty>::operator bool() const
{
    return this->_obj != nullptr;
}

template<typename _Ty>
constexpr auto WeakPtr<_Ty>::get_shared() const -> _SharedPtr
{
    return _SharedPtr(this->load());
}


template<typename __Ty, typename...__Args>
auto make_shared(__Args&&...args) -> SharedPtr<__Ty>
{
    using _CountObj = CountObj<__Ty>;
    using _SharedPtr = SharedPtr<__Ty>;
    auto count_obj = new _CountObj;
    count_obj->construct(std::forward<__Args>(args)...);

    return _SharedPtr(count_obj);
}

} // namespace utils
