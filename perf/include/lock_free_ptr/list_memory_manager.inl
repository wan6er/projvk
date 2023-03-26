#include <mutex>
namespace utils
{


template<typename _Ty, unsigned int _Size>
void ListMemoryManager<_Ty, _Size>::destroy(_CountObj ptr)
{
    // ptr.destory();
}
    
template<typename _Ty, unsigned int _Size>
void ListMemoryManager<_Ty, _Size>::increment(_CountObj objs)
{
    
    for (unsigned int i = 0; i < objs.size; ++i) {
        _MMBase::increment(objs[i]);
        // if (obj != nullptr) {
        //     obj->add_refs(MemoryOrderSeqCst);
        // }
    }
}

template<typename _Ty, unsigned int _Size>
void ListMemoryManager<_Ty, _Size>::increment_relaxed(_CountObj obj)
{
    for (unsigned int i = 0; i < obj.size; ++i) {
        _MMBase::increment_relaxed(obj[i]);
    }
}

template<typename _Ty, unsigned int _Size>
void ListMemoryManager<_Ty, _Size>::decrement(_CountObj objs)
{
    for (unsigned int i = 0; i < objs.size; ++i) {
        // _Base::decrement(obj[i]);
        auto obj = objs[i];
        if (obj != nullptr) { 
            auto _r = obj->ref_cnt.fetch_sub(1);
            CPERF_ASSERT(_r > 0);
            if (_r == 1) {
                atomic_thread_fence(MemoryOrderRelease);
                obj->release();
                objs.destory(i);
            }
        }
    }
}

} // namespace utils
