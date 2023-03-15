
namespace utils
{


template<typename _Ty>
void MemoryManager<_Ty>::destroy(_CountPtr ptr)
{
    delete ptr;
}
    
template<typename _Ty>
void MemoryManager<_Ty>::increment(_CountPtr obj)
{
    if (obj != nullptr) {
        obj->add_refs(MemoryOrderSeqCst);
    }
}

template<typename _Ty>
void MemoryManager<_Ty>::increment_relaxed(_CountPtr obj)
{
    if (obj != nullptr) {
        obj->add_refs(MemoryOrderRelaxed);
    }
}

template<typename _Ty>
bool MemoryManager<_Ty>::decrement(_CountPtr obj)
{

    if (obj != nullptr) { 
        obj->sub_refs(MemoryOrderRelease);
        if (obj->ref_cnt.load(MemoryOrderAcquire) == 0) {
            atomic_thread_fence(MemoryOrderRelease);
            obj->release();
            return true;
        }
    }
    return false;
}

} // namespace utils
