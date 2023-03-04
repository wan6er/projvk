#ifndef __SYNC_STACK_H__
#define __SYNC_STACK_H__

#include <atomic>
#include <memory>
#include <functional>
#include <optional>

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

template<typename _Ty>
class _LockFreeStackFlag
{
public:
    using ValueType = _Ty;
    using NodeType = _Node<ValueType>;
    using NodePtrType = _PtrType<NodeType>;
    // using LockFreeNodePtrType = std::atomic<NodePtrType>;
    
    _PtrDiff head;
    size_t size;

    _LockFreeStackFlag() noexcept :
        head(0), size(0)
    {}
    
    _LockFreeStackFlag(_LockFreeStackFlag const& flag) noexcept = default;

    bool operator==(_LockFreeStackFlag const& right) const noexcept
    {
        return head == right.head;
    }

    auto get_head_ptr() -> NodePtrType
    {
        return reinterpret_cast<NodePtrType>(head);
    }
    
    template<typename __Ty1>
    void set(__Ty1&& headval) 
    {
        head = reinterpret_cast<_PtrDiff>(headval);
    }

protected:

};

template<typename _Ty>
class LockFreeStack
{
    using ValueType = _Ty;
    using NodeType = _Node<ValueType>;
    using NodePtrType = _PtrType<NodeType>;
    using FlagType = _LockFreeStackFlag<_Ty>;
    using LockFreeFlagType = std::atomic<FlagType>;
    using LockFreeNum = std::atomic<size_t>;

public:
    LockFreeStack() noexcept = default;
    LockFreeStack(LockFreeStack const&) = default;
    ~LockFreeStack()
    {
        clean();
    }

    void push(_Ty const& t)
    {
        auto node = new_node(t);
        auto cur_flag = _flag.load();

        while (!_flag.compare_exchange_weak(cur_flag, _unsafe_push(cur_flag, node)));

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
            std::atomic_thread_fence(std::memory_order_acquire);
            del_node(node);
        }
        return ret;
    }
    
    std::optional<NodePtrType> get_head()
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
    FlagType _unsafe_push(FlagType const& flag, NodePtrType node)
    {
        FlagType ret(flag);
        if (ret.size == 0) {
            ret.set(node);
        } else {
            node->next = ret.get_head_ptr();
            ret.set(node);
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
            ret.size--;
            node = ret.get_head_ptr();
            ret.set(node->next);
        }
        return ret;
    }

    auto _announce(std::function<FlagType()> func) -> std::optional<FlagType>
    {
        _ann_lf.fetch_add(1, std::memory_order_acq_rel);
        std::optional<FlagType> ret = func();        
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

    template<typename..._Args>
    NodePtrType new_node(_Args&&...args) {
        NodePtrType ptr = new NodeType(std::forward<_Args>(args)...);
        return ptr;
    }
    
    void del_node(NodePtrType node) {
        // node->next = nullptr;
        delete node;
    }

private:
    LockFreeFlagType _flag;
    LockFreeNum _ann_lf = 0;
};

} // namespace cperf


#endif