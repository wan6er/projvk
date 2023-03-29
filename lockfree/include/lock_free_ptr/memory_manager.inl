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
        auto _r = obj->ref_cnt.fetch_sub(1);
        CLF_ASSERT(_r > 0);
        if (_r == 1) {
            atomic_thread_fence(MemoryOrderRelease);
            obj->release();
            if constexpr (std::is_pointer_v<_CountObj>) {
                delete obj;
            } else {
                obj.destory();
            }
            return true;
        }
    }
    return false;
}

// template<typename _Ty, typename _CountObj>
// bool MemoryManager<_Ty, _CountObj>::decrement(std::enable_if_t<!std::is_pointer_v<_CountObj>, _CountObj> obj)
// {
//     if (obj != nullptr) { 
//         auto _r = obj->ref_cnt.fetch_sub(1);
//         CLF_ASSERT(_r > 0);
//         if (_r == 1) {
//             atomic_thread_fence(MemoryOrderRelease);
//             obj->release();
//             obj.destory();
//             return true;
//         }
//     }
//     return false;
// }

} // namespace utils
