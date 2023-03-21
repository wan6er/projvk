#include <mutex>
namespace utils
{


template<typename _Ty, typename _CountObj>
void MemoryManager<_Ty, _CountObj>::destroy(_CountObj ptr)
{
    // ptr.destory();
}
    
template<typename _Ty, typename _CountObj>
void MemoryManager<_Ty, _CountObj>::increment(_CountObj obj)
{
    if (obj != nullptr) {
        obj->add_refs(MemoryOrderSeqCst);
    }
}

template<typename _Ty, typename _CountObj>
void MemoryManager<_Ty, _CountObj>::increment_relaxed(_CountObj obj)
{
    if (obj != nullptr) {
        obj->add_refs(MemoryOrderSeqCst);
    }
}

template<typename _Ty, typename _CountObj>
bool MemoryManager<_Ty, _CountObj>::decrement(_CountObj obj)
{
    if (obj != nullptr) { 
        // auto _r = obj->ref_cnt.load(MemoryOrderAcquire);
        // while (_r != 0 && !obj->ref_cnt.compare_exchange_weak(_r, _r - 1));
        auto _r = obj->ref_cnt.fetch_sub(1);
        CPERF_ASSERT(_r > 0);
        if (_r == 1) {
            atomic_thread_fence(MemoryOrderRelease);
            obj->release();
            obj.destory();
            return true;
        }
    }
    return false;
}

} // namespace utils
