
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
    this->increment(this->_obj);
}

template<typename _Ty>
constexpr SharedPtr<_Ty>::SharedPtr(void* ptr) :
    SharedPtr(_CountPtr(ptr))
{
    // this->increment(this->_obj);
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
void SharedPtr<_Ty>::swap(_CountPtr const& ptr)
{
    auto _o = this->get_count();
    if (_o != ptr) {
        this->increment(ptr);
        this->decrement(_o);
    }
    this->get_count_ref() = ptr;
}

template<typename _Ty>
void SharedPtr<_Ty>::operator=(SharedPtr const& ptr)
{
    auto _n = ptr.get_count();
    auto _o = this->get_count();
    if (*this != ptr) {
        this->increment(_n);
        this->decrement(_o);
    }
    this->get_count_ref() = _n;
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
template<typename __Ptr>
constexpr auto SharedPtr<_Ty>::operator==(__Ptr&& ptr) const -> bool
{
    return this->get_count() == ptr.get_count();
}

template<typename _Ty>
template<typename __Ptr>
constexpr auto SharedPtr<_Ty>::operator!=(__Ptr&& ptr) const -> bool
{
    return this->get_count() != ptr.get_count();
}


// template<typename _Ty>
// constexpr SharedPtr<_Ty>::operator bool() const
// {
//     return this->_obj != nullptr;
// }

    
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
template<typename __Ptr>
constexpr auto WeakPtr<_Ty>::operator==(__Ptr&& ptr) const -> bool
{
    return this->get_count() == reinterpret_cast<void*>(ptr.get_count());
}

template<typename _Ty>
template<typename __Ptr>
constexpr auto WeakPtr<_Ty>::operator!=(__Ptr&& ptr) const -> bool
{
    return this->get_count() != reinterpret_cast<void*>(ptr.get_count());
}


// template<typename _Ty>
// constexpr WeakPtr<_Ty>::operator bool() const
// {
//     return this->_obj != nullptr;
// }

template<typename _Ty>
constexpr auto WeakPtr<_Ty>::get_shared() const -> _SharedPtr
{
    return _SharedPtr(this->load());
}


template<typename __Ty, typename...__Args>
auto make_cshared(__Args&&...args) -> SharedPtr<__Ty>
{
    using _CountObj = CountObjR<__Ty>;
    using _SharedPtr = SharedPtr<__Ty>;
    
    _CountObj count_obj(make_count_obj<__Ty>());
    count_obj->construct(std::forward<__Args>(args)...);

    return _SharedPtr(count_obj);
}

} // namespace utils
