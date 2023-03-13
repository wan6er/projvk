#ifndef __SYNC_QUEUE_H__
#define __SYNC_QUEUE_H__

#include "sync_structure_base.h"

#include <thread>
#include <iostream>

namespace utils
{


template<typename _Ty>
class _LockFreeQueueFlag
{
public:
    using ValueType = _Ty;
    using NodeType = _Node<ValueType>;
    using NodePtrType = _PtrType<NodeType>;
    
    _PtrDiff head;
    _PtrDiff tail;
    size_t size;

    constexpr _LockFreeQueueFlag() noexcept :
        head(0), tail(0), size(0)
    {}
    
    constexpr _LockFreeQueueFlag(_LockFreeQueueFlag const& flag) noexcept = default;

    constexpr bool operator==(_LockFreeQueueFlag const& right) const noexcept
    {
        return head == right.head && tail == right.tail;
    }

    constexpr auto get_head_ptr() const -> NodePtrType
    {
        return reinterpret_cast<NodePtrType>(head);
    }
    
    constexpr auto get_tail_ptr() const -> NodePtrType
    {
        return reinterpret_cast<NodePtrType>(tail);
    }
    
    template<typename __Ty1>
    void set_head(__Ty1&& headval) 
    {
        head = reinterpret_cast<_PtrDiff>(headval);
    }

    template<typename __Ty1>
    void set_tail(__Ty1&& tailval) 
    {
        tail = reinterpret_cast<_PtrDiff>(tailval);
    }

protected:

};

template<typename _Ty>
class LockFreeQueue : public __SyncStructureBase
{
    using ValueType = _Ty;
    using NodeType = _Node<ValueType>;
    using NodePtrType = _PtrType<NodeType>;
    using NodeCPtrType = _CPtrType<NodeType>;
    using FlagType = _LockFreeQueueFlag<_Ty>;
    using LockFreeFlagType = std::atomic<FlagType>;

public:
    LockFreeQueue() noexcept = default;
    
    LockFreeQueue(LockFreeQueue const&) = delete; 
    // LockFreeQueue(LockFreeQueue const& copy) :
    //     _flag(copy._flag.load()) 
    // {
    // }

    ~LockFreeQueue()
    {
        clean();
    }

    void push(_Ty const& t)
    {
        auto node = new_obj<NodeType>(t);

        NodePtrType tail = nullptr;
        auto cur_flag = _flag.load();

        while (!_flag.compare_exchange_weak(cur_flag, _unsafe_push(cur_flag, node, tail)));

        if (tail) {
            tail->next = node;
        }
    }
    
    std::optional<ValueType> pop()
    {
        NodePtrType node = nullptr;

        _announce([&]() {
            auto cur_flag = _flag.load(std::memory_order_relaxed);
            while (!_flag.compare_exchange_weak(cur_flag, _unsafe_pop(cur_flag, node), std::memory_order_relaxed));
        });

        std::optional<ValueType> ret;
        if (node != nullptr) {
            ret = node->val;
            _wait_announce();
            del_obj(node);
        }
        return ret;
    }
    
    void swap(LockFreeQueue& queue)
    {
        queue._flag.store(_flag.exchange(queue._flag, std::memory_order_release), std::memory_order_acquire);
    }

    std::optional<NodePtrType> get_head()
    {
        auto flag = _flag.load(std::memory_order_relaxed);
        if (flag.size > 0) {
            return flag.get_head_ptr();
        }
        return {};
    }

    std::optional<NodeCPtrType> get_head() const
    {
        auto flag = _flag.load(std::memory_order_relaxed);
        if (flag.size > 0) {
            return flag.get_head_ptr();
        }
        return {};
    }

    auto flag() const -> FlagType 
    {
        return _flag.load();
    }

    size_t size() const
    {
        return _flag.load().size;
    }

    void clean() 
    {
        while (pop().has_value());
    }

protected:
    FlagType _unsafe_push(FlagType const& flag, NodePtrType node, NodePtrType& oldnode)
    {
        FlagType ret(flag);
        if (ret.size == 0) {
            ret.set_head(node);
            ret.set_tail(node);
        } else {
            oldnode = ret.get_tail_ptr();
            ret.set_tail(node);
        }
        ret.size++;
        return ret;
    }
    
    FlagType _unsafe_pop(FlagType const& flag, NodePtrType& node)
    {
        FlagType ret(flag);
        if (ret.size == 0) {
            // error
            node = nullptr;
        } else {
            if (ret.size > 1) {
                node = ret.get_head_ptr();
                while (node->next == nullptr);
                ret.set_head(node->next);
            } else {
                node = ret.get_head_ptr();
                ret.set_head(nullptr);
                ret.set_tail(nullptr);
            }
            --ret.size;
        }
        return ret;
    }


private:
    LockFreeFlagType _flag;
};

} // namespace cperf


#endif