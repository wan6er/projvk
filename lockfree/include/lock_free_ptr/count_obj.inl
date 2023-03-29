
namespace utils
{
    

template<typename _Ty>
auto CountObj<_Ty>::get() -> _Ty*
{
    return reinterpret_cast<_Ty*>(storage);
}

template<typename _Ty>
auto CountObj<_Ty>::get() const -> _Ty const*
{
    return reinterpret_cast<_Ty const*>(storage);
}

template<typename _Ty>
auto CountObj<_Ty>::add_refs(MemoryOrder order) -> RefCount
{
    return ref_cnt.fetch_add(1, order);
}

template<typename _Ty>
auto CountObj<_Ty>::sub_refs(MemoryOrder order) -> RefCount
{
    return ref_cnt.fetch_sub(1, order);
}

template<typename _Ty>
template<typename...__Args>
void CountObj<_Ty>::construct(__Args&&...args)
{
    new (get()) _Ty(std::forward<__Args>(args)...);
}

template<typename _Ty>
void CountObj<_Ty>::construct()
{
    new (get()) _Ty;
}

template<typename _Ty>
void CountObj<_Ty>::release()
{
    get()->~_Ty();
}


} // namespace utils
