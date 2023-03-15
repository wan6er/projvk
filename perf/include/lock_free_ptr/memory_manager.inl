
namespace utils
{


template<typename _Ty>
void MemoryManager<_Ty>::destroy(_CountObjPtr ptr)
{
    delete ptr;
}
    
template<typename _Ty>
void MemoryManager<_Ty>::increment(_CountObjPtr obj)
{
    if (obj != nullptr) {
        obj->add_refs(std::memory_order_seq_cst);
    }
}

template<typename _Ty>
void MemoryManager<_Ty>::increment_relaxed(_CountObjPtr obj)
{
    if (obj != nullptr) {
        obj->add_refs(std::memory_order_relaxed);
    }
}

template<typename _Ty>
bool MemoryManager<_Ty>::decrement(_CountObjPtr obj)
{

    if (obj != nullptr) { 
        if (obj->ref_cnt.load(std::memory_order_release) == 1) {
            obj->release();
            return true;
        }
        obj->sub_refs(std::memory_order_relaxed);
    }
    return false;
}

} // namespace utils
