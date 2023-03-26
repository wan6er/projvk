#ifndef __SYNC_STRUCTURE_BASE_H__
#define __SYNC_STRUCTURE_BASE_H__

#include "perf_basic.h"

#include "lock_free_ptr/shared_ptr.h"
#include "lock_free_ptr/lock_free_ptr.h"
#include "compatible_atomic.h"
#include <optional>
#include <functional>
#include <iostream>

namespace utils
{

template<typename _Ty>
using _PtrType = _Ty*;

template<typename _Ty>
using _CPtrType = _Ty const*;

// template<typename _Ty>
// using _OwnPtrType = std::unique_ptr<_Ty>;
using _PtrDiff = std::ptrdiff_t;


template<typename _Ty>
struct _Node
{
    using ValueType = _Ty;
    using NodeType = _Node<ValueType>;
    using NodePtr = SharedPtr<NodeType>;

    _Ty val;
    NodePtr next;

    _Node() noexcept = default;
    _Node(_Ty const& val, NodePtr next) : val(val), next(next) {}
    _Node(_Ty const& val) : _Node(val, nullptr) {}
    _Node(_Node const& val) = default;

    ~_Node() {}

    operator ValueType() const { return val; }

};

// template<typename _Ty>
// struct _LockFreeNode
// {
//     using ValueType = _Ty;
//     using NodeType = _LockFreeNode<ValueType>;
//     using NodePtr = SharedPtr<NodeType>;

//     _Ty val;
//     NodePtr next;

//     _LockFreeNode() noexcept = default;
//     _LockFreeNode(_Ty const& val, NodePtr next) : val(val), next(next) {}
//     _LockFreeNode(_Ty const& val) : _LockFreeNode(val, nullptr) {}
//     _LockFreeNode(_LockFreeNode const& val) = default;

//     ~_LockFreeNode() {}

//     operator ValueType() const { return val; }

// };

// template<typename _Ty>
// struct _DoubleNode
// {
//     using ValueType = _Ty;
//     using NodeType = _Node<ValueType>;
//     using NodePtr = LockFreePtr<NodeType>;

//     _Ty val;
//     NodePtr last;
//     NodePtr next;

//     _DoubleNode() noexcept = default;
//     _DoubleNode(_Ty const& val, NodePtr last, NodePtr next) : val(val), last(last), next(next) {}
//     _DoubleNode(_Ty const& val) : _DoubleNode(val, nullptr, nullptr) {}
//     _DoubleNode(_DoubleNode const& val) = default;

//     ~_DoubleNode() {}

//     operator ValueType() const { return val; }

// };

class __SyncStructureBase
{
    // using LockFreeNum = Atomic<size_t>;
protected:

    // template<typename _RetType>
    // auto _announce(std::function<_RetType()> func) -> std::optional<_RetType>
    // {
    //     _ann_lf.fetch_add(1, MemoryOrderAcqRel);
    //     std::optional<_RetType> ret = func();        
    //     _ann_lf.fetch_sub(1, MemoryOrderAcqRel);
    //     return ret;
    // }

    // void _announce(std::function<void()> func)
    // {
    //     _ann_lf.fetch_add(1, MemoryOrderAcqRel);
    //     func();
    //     _ann_lf.fetch_sub(1, MemoryOrderAcqRel);
    // }

    // void _wait_announce(size_t wait_time = 0)
    // {
    //     while (_ann_lf.load(MemoryOrderRelaxed) != 0);
    // }


private:
    // LockFreeNum _ann_lf = 0;

};

} // namespace utils


#endif