#ifndef __SYNC_DEQUE_H__
#define __SYNC_DEQUE_H__

#include <atomic>
#include <memory>
#include <optional>

namespace utils
{

template<typename _Ty>
using _PtrType = _Ty*;
template<typename _Ty>
using _OwnPtrType = std::unique_ptr<_Ty>;
using _PtrDiff = std::ptrdiff_t;


template<typename _Ty>
struct _Node
{
    using ValueType = _Ty;
    using NodeType = _Node<ValueType>;
    using NodePtrType = _PtrType<NodeType>;

    _Ty val;
    NodePtrType next;

    constexpr _Node() noexcept = default;
    constexpr _Node(_Ty const& val, NodePtrType next) : val(val), next(next) {}
    constexpr _Node(_Ty const& val) : _Node(val, nullptr) {}
    constexpr _Node(_Node const& val) = default;

};

template<typename _Ty>
class _SyncDequeFlag
{
public:
    using ValueType = _Ty;
    using NodeType = _Node<ValueType>;
    using NodePtrType = _PtrType<NodeType>;
    // using LockFreeNodePtrType = std::atomic<NodePtrType>;
    
    _PtrDiff head;
    _PtrDiff tail;
    size_t size;

    constexpr _SyncDequeFlag() noexcept :
        head(0), tail(0), size(0)
    {}
    
    constexpr _SyncDequeFlag(_SyncDequeFlag const& flag) noexcept = default;

    constexpr bool operator==(_SyncDequeFlag const& right) const noexcept
    {
        return head == right.head && tail == right.tail;
    }

    template<typename __Ty>
    constexpr void set_head(__Ty&& val) 
    {
        head = (_PtrDiff)val;
    }
    
    constexpr auto get_head_ptr() -> NodePtrType
    {
        return reinterpret_cast<NodePtrType>(head);
    }

    template<typename __Ty>
    constexpr void set_tail(__Ty&& val) 
    {
        tail = (_PtrDiff)val;
    }

    constexpr auto get_tail_ptr() -> NodePtrType
    {
        return reinterpret_cast<NodePtrType>(tail);
    }
    
    template<typename __Ty1, typename __Ty2>
    constexpr void set(__Ty1&& headval, __Ty2&& tailval) 
    {
        set_head(std::forward<__Ty1>(headval));
        set_tail(std::forward<__Ty2>(tailval));
    }
    
    constexpr void set() 
    {
    }

protected:


};

template<typename _Ty>
class SyncQueue
{
    using ValueType = _Ty;
    using NodeType = _Node<ValueType>;
    using NodePtrType = _PtrType<NodeType>;
    using FlagType = _SyncDequeFlag<_Ty>;
    using LockFreeFlagType = std::atomic<FlagType>;

public:
    constexpr SyncQueue() noexcept = default;
    constexpr SyncQueue(SyncQueue const&) = default;

    constexpr void push_back(_Ty const& t)
    {
        auto node = new_node(t);
        auto cur_flag = _flag.load();
        NodePtrType cur_tail = nullptr;
        while (!_flag.compare_exchange_weak(cur_flag, _unsafe_push_back(cur_flag, node, cur_tail)));

        if (cur_tail != nullptr) {
            cur_tail->next = node;
        }
    }
    
    constexpr std::optional<ValueType> pop_front()
    {
        NodePtrType node = nullptr;
        auto cur_flag = _flag.load();
        while (!_flag.compare_exchange_weak(cur_flag, _unsafe_pop_front(cur_flag, node)));

        if (node != nullptr) {
            auto ret = node->val;
            del_node(node);
            return ret;
        }
        return {};
    }
    
    constexpr std::optional<NodePtrType> get_head()
    {
        auto flag = _flag.load();
        if (flag.size > 0) {
            return flag.get_head_ptr();
        }
        return {};
    }

    constexpr auto flag() const -> FlagType 
    {
        return _flag.load();
    }

    constexpr size_t size() const
    {
        return _flag.load().size;
    }

protected:
    constexpr FlagType _unsafe_push_back(FlagType const& flag, NodePtrType node, NodePtrType& tail)
    {
        FlagType ret(flag);
        if (ret.size == 0) {
            ret.set(node, node);
        } else {
            // ret.get_tail_ptr()->next = node;
            tail = ret.get_tail_ptr();
            ret.set_tail(node);
        }
        ret.size++;
        return ret;
    }
    
    constexpr FlagType _unsafe_pop_front(FlagType const& flag, NodePtrType& node)
    {
        FlagType ret(flag);
        if (ret.head == 0 || ret.tail == 0) {
            // error
            node = nullptr;
        } else {
            ret.size--;
            node = ret.get_head_ptr();
            ret.set_head(ret.get_head_ptr()->next);
            if (ret.size == 0) {
                ret.set_tail(0);
            }
        }
        return ret;
    }

    template<typename..._Args>
    constexpr NodePtrType new_node(_Args&&...args) {
        NodePtrType ptr = new NodeType(std::forward<_Args>(args)...);
        return ptr;
    }
    
    constexpr void del_node(NodePtrType node) {
        delete node;
    }

private:
    LockFreeFlagType _flag;

};

} // namespace cperf


#endif