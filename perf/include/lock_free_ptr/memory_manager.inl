
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
        obj->add_refs(MemoryOrderSeqCst);
    }
}

template<typename _Ty>
bool MemoryManager<_Ty>::decrement(_CountPtr obj)
{
    if (obj != nullptr) { 
        auto _r = obj->ref_cnt.load(MemoryOrderAcquire);
        while (_r != 0 && !obj->ref_cnt.compare_exchange_weak(_r, _r - 1));

        // CPERF_ASSERT(_r != 0);
        if (_r == 1) {
            atomic_thread_fence(MemoryOrderRelease);
            obj->release();
            return true;
        }
    }
    return false;
}

} // namespace utils
