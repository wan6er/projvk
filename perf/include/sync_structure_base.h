#ifndef __SYNC_STRUCTURE_BASE_H__
#define __SYNC_STRUCTURE_BASE_H__

#include <atomic>
#include <optional>
#include <functional>

namespace utils
{

template<typename _Ty>
using _PtrType = _Ty*;
// template<typename _Ty>
// using _OwnPtrType = std::unique_ptr<_Ty>;
using _PtrDiff = std::ptrdiff_t;


template<typename _Ty>
struct _Node
{
    using ValueType = _Ty;
    using NodeType = _Node<ValueType>;
    using NodePtrType = _PtrType<NodeType>;

    _Ty val;
    NodePtrType next;

    _Node() noexcept = default;
    _Node(_Ty const& val, NodePtrType next) : val(val), next(next) {}
    _Node(_Ty const& val) : _Node(val, nullptr) {}
    _Node(_Node const& val) = default;

};

class __SyncStructureBase
{
    using LockFreeNum = std::atomic<size_t>;

protected:

    template<typename _RetType>
    auto _announce(std::function<_RetType()> func) -> std::optional<_RetType>
    {
        _ann_lf.fetch_add(1, std::memory_order_acq_rel);
        std::optional<_RetType> ret = func();        
        _ann_lf.fetch_sub(1, std::memory_order_acq_rel);
        return ret;
    }

    void _announce(std::function<void()> func)
    {
        _ann_lf.fetch_add(1, std::memory_order_acq_rel);
        func();
        _ann_lf.fetch_sub(1, std::memory_order_acq_rel);
    }

    void _wait_announce(size_t wait_time = 0)
    {
        while (_ann_lf.load(std::memory_order_relaxed) != 0);
    }

    template<typename _Ty, typename..._Args>
    _Ty* new_obj(_Args&&...args) {
        return new _Ty(std::forward<_Args>(args)...);
    }
    
    template<typename _Ty>
    void del_obj(_Ty*& node) {
        _Ty* tmp = node;
        node = nullptr;
        std::atomic_thread_fence(std::memory_order_acquire);
        delete tmp;
    }

private:
    LockFreeNum _ann_lf = 0;

};

} // namespace utils


#endif